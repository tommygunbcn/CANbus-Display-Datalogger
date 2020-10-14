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

/*this file defines the joystick funtions. TBR*/

#include "Global.h"



int Xvalue = 0;  // adc value  for the height
int Yvalue = 0;  // adc counts for the width

bool signal_right_detected = 0;  // set when right detected
bool signal_left_detected = 0;   // set when left detected
bool signal_up_detected = 0;      // set when up detected
bool signal_down_detected = 0;    // set when down detected
bool signal_button_pressed = 0;   // set when button pressed detected

void JoystickPinIsr()
{
	/*
	TBR.Interrupt service request for the push button of the joystick.
	when editing parameters, bear in ming that the parameter will be effectivily changed when the controller answers the requests. For most controller, after editing one 
	parameter, they must be powered up again, so only ONE PARAMETER can be changed at a time.
	*/


	if (line_higlight)//current line is ready to be edited, so we are in an editable screen
	{
		if (select_line) //user has clickd already in a line, 
		{
			send_config_CAN_mssg = 1;  //set flag so that a config message will be sent

			if (current_screen_num == 5) /*TBR*/
			{
				if (line_higlight == 1)
				{
					change_ki = 1;
				}
				else if (line_higlight == 2)
				{
					change_kp = 1;
				}
				else if (line_higlight == 3)
				{
					change_max_gate_var = 1;
				}
				else if (line_higlight == 4)
				{
					change_bemf_trim = 1;
				}
				else if (line_higlight == 5)
				{
					/*do nothing*/
				}
			}

		}
		else
		{
			signal_button_pressed = 1; //set flag so that to signal the user wants to edit this line

		}

	}
	else
	{
		CheckSecretCombinationClick();
	}


}
void check_joystick(void)
{
	/*TBR.This functions is called at the same rate that the refresh screen rate. Will manage the editable screen,as well as the secret combination.
	when in editable screen, use the up and down movement to navigate between lines, they will turn yellow when selected. Once in a selected line, click on the joystick to select its value,
	it will turn yellow too. In this state, use up and down to modify value. Once modified, click once more to send the config parameter to the respective node. When the nodes anwers that message acknowledging
	the change, the value will turn white again*/

	// check the button pressed when in editing menu
	if ((line_higlight) && (signal_button_pressed))
	{
		select_line = 1;   //flag that line is to be edited, since user has clicked in highlighted line
		signal_button_pressed = 0; //reset flag

	}

	//check the width reading, this is what is used to navigate between the screens
	if (Yvalue >= RIGHT_BOUNDARY)
	{
		signal_right_detected = 1;
		Increment_screen_num();

	}
	else if (Yvalue <= LEFT_BOUNDARY)
	{
		signal_left_detected = 1;
		Decrement_screen_num();
	}
	//check the height reading:important in the editing parameters or the secret combination
	if (Xvalue <= UP_BOUNDARY)
	{
		signal_up_detected =TRUE;// flag up movement detected, 
		
	}
	else if (Xvalue >= DOWN_BOUNDARY)
	{
		signal_down_detected = 1;

	}
	else// no signal detected
	{
	/*do nothing*/
	}
	//now check what to do with the editable screen or the secret combination
	if (!CheckEditableScreen())  // only enter the secret combination if not in an editable screen
	{
		CheckSecretCombinationStates();

	}

}

bool CheckEditableScreen()
{
	/*TBR:This function will check the special situation of an editable screen, and return "true" if indeed we are in that case. 
	It is called after a joystick up or down movement. the strings are updated here */
	bool result = FALSE;
	
	if (current_screen_num == 5)		// has to be edited if the editable screen changes!
	{
		if (!select_line) //no line selected yet.
		{
		
			Timer1.stop();//stop timer so does not count time without messages and detects a CAN message timeout. Also note that screen will be printed again, we dont want to
							// start a new screen print while printing
			if (signal_up_detected)  
			{
				if (line_higlight > 1)
				{
					line_higlight--;
				}
				else// print last line
				{
					line_higlight = 5;
				}
				current_screen.print_screen();//print screen so that next line will be highlighted
				signal_up_detected = FALSE;//reset flag
			}
			else if (signal_down_detected)
			{
				if (line_higlight < 5)
				{
					line_higlight++;
				}
				else// print first line
				{
					line_higlight = 1;
				}
				signal_down_detected = FALSE;//reset flag
				current_screen.print_screen();//print screen so that next line will be highlighted
			}
		
			
			Timer1.resume();
		}
		else//line already selected, edit values
		{
			if (line_higlight == 1)  /*modify values, between a max and a minimum*/
			{
				if (signal_up_detected)
				{
					if (ki < 250)     //max value , TBD
					{
						ki++;

					}
					signal_up_detected = FALSE;//reset flag
				}
				else if (signal_down_detected)
				{
					if (ki > 0)     // min value TBD
					{
						ki--;

					}
					signal_down_detected = FALSE;//reset flag
				}

				dtostrf(ki, 2, 0, received_number);
				cntr_Ki_str = (String)received_number;
			}
			else if (line_higlight == 2)
			{
				if (signal_up_detected)
				{
					if (kp < 250)     //max value , TBD
					{
						kp++;

					}
					signal_up_detected = FALSE;//reset flag
				}
				else if (signal_down_detected)
				{
					if (kp > 0)     // min value TBD
					{
						kp--;

					}
					signal_down_detected = FALSE;//reset flag
				}

				dtostrf(kp, 2, 0, received_number);
				cntr_Kp_str = (String)received_number;
			}
			else if (line_higlight == 3)
			{
				if (signal_up_detected)
				{
					if (max_gate_var < 25)     //max value , TBD
					{
						max_gate_var++;

					}
					signal_up_detected = FALSE;//reset flag
				}
				else if (signal_down_detected)
				{
					if (max_gate_var > 0)     // min value TBD
					{
						max_gate_var--;

					}
					signal_down_detected = FALSE;//reset flag
				}

				dtostrf(max_gate_var, 2, 0, received_number);
				cntr_max_gate_time_str = (String)received_number;
			}
			else if (line_higlight == 4)
			{
				if (signal_up_detected)
				{
					if (bemf_trim < 25)     //max value , TBD
					{
						bemf_trim++;

					}
					signal_up_detected = FALSE;//reset flag
				}
				else if (signal_down_detected)
				{
					if (bemf_trim > 0)     // min value TBD
					{
						bemf_trim--;

					}
					signal_down_detected = FALSE;//reset flag
				}

				dtostrf(bemf_trim, 2, 0, received_number);
				cntr_bemf_trim_str = (String)received_number;
			}

		}
		result = TRUE; // set flag so that we are in an editable screen.
	}
	/*else if ()// to be edited if another editable screen is present
	{
		result = TRUE;
	}*/
	else
	{
		//not in a editable screen
		result = FALSE;
	}
	return(result);
}