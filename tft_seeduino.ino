/*
2019 Copyright (c) microvoltia s.l.

author: Tommygunbcn

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU
Lesser General Public License for more details.*/

/*
This project will continuosly check the CAN bus , and will print the information in a tft display. Project based in the arduino uno platform. ( seeeduino in this case)
the display used in this project is the 2,8" TFT touch shield, v2.1 by seeed studio, the touchscreen option is not used. This display communicates with the arduino via SPI, the setup is made in TFTv2.h
The library provided has been slightly modified to meet the project requirements.
the CAN bus module used is the Can BUS shield v1.2 by seeed studio, also connecting via SPI, and the setup is made in CAN_setup.h. In this project no CAN bus ending resistor is present.
There is also a measure sensed via an analog pin--> analog.h
The navigation between screens is made via a Joystick


The normal screen consists in a big number ( speed) , and 5 lines, each line showing one title and one value. ex:  current[A]: 32.
There is also a screen for displaying 5 lines of text, useful for displaying error situations.
There is also a screen that can edit its values.
Also, there is a strip in the bottom that will inform about any error or warning situation
Also, there is the possibility of sending via serial a frame of 64 bytes in a regular basis.
And also, a secret screen can be reached with a secret combination of the joystick.

In this project, 3 nodes are used.
For adding/erasing screen every file has to be checked and modified acordingly, where the text TBR (To be Revised)
enjoy! :)
*/

#include "global.h"


/*every timer define shall have its corresponding counter*/
// timer defines
#define SCREEN_REFRESH_RATE 500		  // in ms, the refresh of the variables in the screen
#define SERIAL_SEND_RATE 70		      // in ms, the refresh of the variables to be sent via serial
#define SERIAL_SEND_RATE_SLOW 2000	 // in ms, the refresh of the slow varying variables to be sent via serial
#define INFO_REFRESH_RATE 3000		 // in ms, the refresh of the info line
#define CAN_REQUEST_RATE 10			 //in ms, the rate we will send CAN messages for requesting info
#define CAN_TIMEOUT_RATE 1000   	// in ms, if a can message is not received in this time, it will be considered a timeout. 


// timer counters
volatile unsigned int counter_timer_screen = 0;
volatile unsigned int counter_timer_info = 0;
volatile unsigned int counter_timer_serial = 0;
volatile unsigned int counter_timer_serial_slow = 0;
volatile unsigned int counter_timer_CAN_request = 0;
volatile unsigned int counter_CAN_timeout = 0;



void setup()
{
	//set the digital pin of the joystick, pullup and set the corresponding isr
	pinMode(pinJoyButton, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(pinJoyButton),JoystickPinIsr, LOW);
	//start serial and star CAN. 
#ifdef SERIAL_COM
	Serial.begin(SERIAL_BAUD_RATE);
#endif
	while (CAN_OK != CAN.begin(CAN_SPEED))             
	{
#ifdef SERIAL_COM
		Serial.println("CAN ko");
		Serial.println("Init again");
		delay(100);
#endif	
	}
#ifdef SERIAL_COM
	Serial.println("CAN ok");
#endif	
	   	 
	//init TFT
	TFT_BL_ON;      // turn on the background light
	Tft.TFTinit();  // init TFT library
		
	// print lateral buttons
	Tft.drawString(right_str, RIGHT, ((Tft.getDisplayYSize()) / 2),3, arrow_color,BLACK);
	Tft.drawString(left_str, LEFT, ((Tft.getDisplayYSize()) / 2), 3, arrow_color, BLACK);

	// set current screen object with the first screen
	current_screen = screen_0;
	// set the current screen number
	Set_current_screen(current_screen_num);
	//print screen
	current_screen.print_screen();
	
	//init timer
	Timer1.initialize(1000); // set a timer to tick every ms
	Timer1.attachInterrupt(timerIsr); // attach the service routine here

	//initialize any parameter here, even though initialized in the definition, better make sure. 
	/*TBR if the screen with editable parameters changes.*/
	Pi_parameters.signal.ki = 0x00;
	general_parameters.signal.version = 0x00;
}

void loop()
{
	/*
	The main task, performed every loop. As the screen printing is blocking, we cannot stablish a main loop in a timed fashion,
	hence all the timings(uart sending rate for example) are only aprox.
	*/
		
	//principal task, do it every loop
	Check_CAN_Task();

	/*do other tasks, only one at a time and ordered so that the most important are the first*/
	// the serial task is the most important of the secondary tasks , as the timming is important
	if (Flag_check_serial_comm)
	{
#ifdef SERIAL_COM
		Check_Serial_Task();
#endif	
		Flag_check_serial_comm = 0;

	}
	//do printing tasks and check joystick
	else if (Flag_refesh_screen)
	{
		// reset all the flags
		signal_right_detected = 0;
		signal_left_detected = 0;
		signal_up_detected = 0;
		signal_down_detected = 0;

		//read analog values
		Xvalue = analogRead(pinJoyX);
		//give some time between readings, meanwhile, draw the speed
		Tft.drawString(Speed_str, CENTER, SPEED_Y_OFFSET, SPEED_SIZE, speed_color,BLACK);
		//read analog  value
		Yvalue = analogRead(pinJoyY);
		
		//decide wich movement has been detected, also  deal with the editable screen  and the  secret combination
		check_joystick();
		

		//navigate between screens, if lateral buttons have been touched
		if (signal_right_detected || signal_left_detected)
		{
			Timer1.stop();//stop timer so does not count time without messages, and detects a CAN timeout. It si considered that the user will not be constantly changing screec
			//paint the respective arrow in blue to signal the user the joystick movement has been received. The arrow in blu will last until the screen has been printed
			arrow_color = BLUE;
			if (signal_right_detected)
			{
				Tft.drawString(right_str, RIGHT, ((Tft.getDisplayYSize()) / 2), 3, arrow_color, BLACK);
			}
			else if(signal_left_detected)
			{
				Tft.drawString(left_str, LEFT, ((Tft.getDisplayYSize()) / 2), 3, arrow_color, BLACK);
			}
			//update and print screen
			Set_current_screen(current_screen_num);
			current_screen.print_screen();

			//print arrows again
			arrow_color = WHITE;
			if (signal_right_detected)
			{
				Tft.drawString(right_str, RIGHT, ((Tft.getDisplayYSize()) / 2), 3, arrow_color, BLACK);
			}
			else if (signal_left_detected)
			{
				Tft.drawString(left_str, LEFT, ((Tft.getDisplayYSize()) / 2), 3, arrow_color, BLACK);
			}
			Timer1.resume();
		}
		//whatever the result of the joystick has been, update the current screen
		Set_current_screen(current_screen_num);
		current_screen.update_screen();
		//check the error or warning situation
		check_info_line();
		
		Flag_refesh_screen = 0;// reset flag
	}
	else if (Flag_refresh_info)
	{
		// update the error string if needed
		Update_info_line();

		Flag_refresh_info = 0; //reset flag
	}

}




void timerIsr()
{
	/*will increment counters and set the corresponding flags. TBR if a node is added or deleted*/
	
	unsigned int aux1;
	//increment all timers
	counter_timer_CAN_request++;
	counter_timer_screen++;
	counter_timer_info++;
	counter_timer_serial++;
	counter_timer_serial_slow++;
	counter_CAN_timeout++;
	//check timers and update flags
	if (counter_timer_CAN_request >= CAN_REQUEST_RATE)
	{
		counter_timer_CAN_request = 0;
		Flag_CAN_request = 1;
	}
	if (counter_timer_serial >= SERIAL_SEND_RATE)
	{
		counter_timer_serial = 0;
		Flag_check_serial_comm = 1;
	}
	if (counter_timer_serial_slow >= SERIAL_SEND_RATE_SLOW)
	{
		counter_timer_serial_slow = 0;
		Flag_check_slow_serial_comm = 1;
	}
	if (counter_timer_screen >= SCREEN_REFRESH_RATE)
	{
		counter_timer_screen = 0;	//reset timer
		Flag_refesh_screen = 1;

	}
	if (counter_timer_info >= INFO_REFRESH_RATE)
	{
		counter_timer_info = 0;	//reset timer
		Flag_refresh_info = 1;

	}
	if (counter_CAN_timeout >= CAN_TIMEOUT_RATE) /*TBR if a node is added or deleted*/
	{
		counter_CAN_timeout =0;
		if (!Flag_ped_can_wdt)		// pedelec can messaging timeout, set everything to 0
		{
			Flag_no_pedelec_can = 1;		// will be set as long as no can message arrives

		}
		Flag_ped_can_wdt = 0;	// reset 

		if (!Flag_bms_can_wdt)		// bms can messaging timeout, 
		{
			analog_2_received = 0;		// reset the flag, will act like in startup
			Flag_no_bms_can = 1;		// will be set as long as no can message arrives
		}
		Flag_bms_can_wdt = 0;	// reset 


		if (!Flag_controller_can_wdt)		// pedelec can messaging timeout, set everything to 0
		{
			//set speed signal to zero so the current speed does not stays freezed in the screen
			speed_int = 0;
			Flag_no_controller_can = 1;		// will be set as long as no can message arrives
		}
		Flag_controller_can_wdt = 0;	// reset 
	}
	

}


// END FILE