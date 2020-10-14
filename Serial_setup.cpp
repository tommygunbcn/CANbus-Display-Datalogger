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
In this file all the serial communication is defined. TBR if differen protocol or different messages are to be sent
*/

#include "global.h"

//declare the machine state variable
unsigned char	what_to_send = do_not_send;
//declare the buffer for sending. remember the length of serial message is 64 bytes
char envia[70];
//received message buffer
unsigned char received[10];

//functions
void Check_Serial_Task(void)
{
	/*This task will check, in a timed manner defined in SERIAL_SEND_RATE, if a message has arrived from the other end or if the arduino should send a message.
	The communication protocol goes like this:
	-At the beggining no messages are sent.
	- The other end of the communication will ask for a start info, only needed once, like sw version, etc.. if will send a simple message, in its first byte: "send_controller_params_char"
	- the arduino , once received this message will send ONCE the requested info.
	- the other end, once processed the first info, will send then a new message,i n its first byte: "send_normal_stuff_char"
	-from now on, the arduino will send every "SERIAL_SEND_RATE" ms the normal message
	- as it are some variables which are slow varying, like the temperature, every "SERIAL_SEND_RATE_SLOW" ms  the arduino will send the normal message+a tail with this slow varying info*/
	int i = 0;

	//check if new message is on the buffer, just read it, no need to know whats in it until a proper comunication with bl antenna
	if (Serial.available() > 0)
	{
		i = 0;

		while (Serial.available() > 0) {	// read the whole buffer
		
			received[i] = Serial.read();
			i++;
		}

		switch (received[0])
		{
		case send_normal_stuff_char:
			what_to_send = send_normal_stuff;
			break;
		case send_controller_params_char:
			what_to_send = send_controller_params;
			break;
		default:
			what_to_send = do_not_send;
			break;

		}

	}
	else//send stuff
	{
		switch (what_to_send)
		{
		case do_not_send:
			break;
		case send_normal_stuff:
		{

			dtostrf(send_normal_stuff, 1, 0, envia);	    //1		

			strcat(envia, Speed_str);						//3	
			strcat(envia, Serial_BMS_Volt_str);				//7
			strcat(envia, Serial_BMS_curr_str);				//11
			strcat(envia, Serial_Cntr_Volt_str);			//14
			strcat(envia, Serial_Cntr_curr_str);		    //17
			strcat(envia, Cntr_ph_current_req_str);			//20
			strcat(envia, Cntr_ph_current_str);				//23
			strcat(envia, cnt_ERROR_Word_1_str);			//28
			strcat(envia, Cntr_breaking_str);				//29
			strcat(envia, cnt_ERROR_Word_2_str);			//34
			strcat(envia, Cntr_mod_index_str);              //37  
			strcat(envia, cnt_WARNING_Word_1_str);			//42
			strcat(envia, Driving_mode_str);				//43
			strcat(envia, ph_current_measured_arms_str);	//46

			if (Flag_check_slow_serial_comm)
			{
				strcat(envia, BMS_soc_str);					//49
				strcat(envia, BMS_temp_str);				//51
				strcat(envia, Mot_temp_str);				//54
				strcat(envia, Cntr_temp_str);				//57
				strcat(envia, Cntr_trip_str);				//60

				Flag_check_slow_serial_comm = 0;  //reset flag

			}
			//close string
			strcpy_P(received_number, (end_str));	//62
			strcat(envia, received_number);
			Serial.flush();
			Serial.write(envia);

		}
		break;

		case send_controller_params: //shouldnt be sent if params are still unknown, if controller has not sent anything yet!
									// doesn't really need to be quick, because it is done just one time, hence, the string building can be done here.
		{
			dtostrf(send_controller_params, 1, 0, envia);	//1		

			dtostrf(contr_version_high, 2, 0, received_number);	//3		
			strcat(envia, received_number);

			dtostrf(contr_version_low, 2, 0, received_number);	//5		
			strcat(envia, received_number);

			dtostrf(beta_version, 1, 0, received_number);	//6		
			strcat(envia, received_number);

			dtostrf(RX_Ctl_brdcst_0.signal.Mileage, 5, 0, received_number);	//11
			strcat(envia, received_number);

			strcat(envia, BMS_cycles_str);	//15

			dtostrf(RX_Ctl_brdcst_0.signal.working_hours, 4, 0, received_number);	//19 
			strcat(envia, received_number);

			//close string
			strcpy_P(received_number, (end_str));	
			strcat(envia, received_number);
			Serial.flush();
			Serial.write(envia);

			what_to_send = do_not_send;//send controller params just once
		}
		break;
		}


	}
}