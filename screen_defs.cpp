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
In this file the class SCREEN is implemented and all the functions regarding the screen operations will also be defined.
The file screen_defs.h contains the declarations of such functions and the external variable declaration.

*/

#include "global.h"



unsigned short speed_color = WHITE;  // the color of the speed shown in the display
unsigned char current_info_display = 0;		// the info currently being displayed

unsigned char current_screen_num = 0;	// the current screen

// screen variables
unsigned char speed_y_size = SPEED_SIZE * FONT_Y + SPEED_Y_OFFSET;		// the y size of the speed indicator
unsigned int y_start, x_end, y_end;
unsigned int x_start = LATERAL_BUTTON_WIDTH;
unsigned int line1_y, line2_y, line3_y, line4_y, line5_y;

// lines
String line_1_str = "";
String line_2_str = "";
String line_3_str = "";
String line_4_str = "";
String line_5_str = "";

String no_str = " ";
volatile unsigned int number_of_errors = 0;
volatile unsigned int number_of_warnings = 0;
unsigned char line_higlight = 0; //If a value on the screen is to be edited, it will be highlighted first. This variable will have the number of the line highlighted
volatile bool select_line = 0; // this flag will be set when user clicks on a highlighted line
bool send_config_CAN_mssg = 0; //this flag will be set when a configuring CAN message is pending to be sent

//security combination
byte   security_state = STATE_WAITING;	// to be able to change parameteres
char   state_open_counter = 0;
unsigned short arrow_color = WHITE; // the arrow color to signal the secret combination
//strings
// this is the buffer of the messaged shown in the info line, in case there are more than one, they will be displayed sequentially
//buffer for errors/warning
String Display[MAX_INFO_LINE] = { "","","","","" };


//define constant strings in program memory: the do not occupy RAM mem, but they are slower, as they have to be read to a buffer
const char On_str[] PROGMEM = { "On" };
const char Off_str[] PROGMEM = { "Off" };
const char space_str[] PROGMEM = { "     " };
const char release_str[] PROGMEM = { "rel" };
const char beta_str[] PROGMEM = { "beta" };
const char end_str[] PROGMEM = { "\r\n" };
//strings in RAM. they are faster to be printed
char right_str[] = { ">" };
char left_str[] = { "<" };


//constructors
SCREEN::SCREEN()	
{

}

SCREEN::SCREEN(unsigned char number, const char* screen_title, const char* line1, const char* line2, const char* line3, const char* line4, const char* line5,
				String * value_1, String * value_2, String * value_3, String * value_4, String * value_5)	
{
	num = number;// the number of the screen to be printed
	title = screen_title;
	line_1 = line1;
	line_2 = line2;
	line_3 = line3;
	line_4 = line4;
	line_5 = line5;

	//calculate the length of the lines

	line1_x = (strlen_P(line1)* FONT_SIZE_PIXELS)+ x_start;
	line2_x = (strlen_P(line2)* FONT_SIZE_PIXELS) + x_start;
	line3_x = (strlen_P(line3)* FONT_SIZE_PIXELS) + x_start;
	line4_x = (strlen_P(line4)* FONT_SIZE_PIXELS) + x_start;
	line5_x = (strlen_P(line5)* FONT_SIZE_PIXELS) + x_start;

	//
	y_start = speed_y_size;
	

	//set lines so that they are equally spaced
	
	line1_y = y_start + SCREEN_TITLE_Y_OFFSET + Tft.getFontYsize() + SCREEN_LINE_Y_OFFSET;

	
	line2_y = y_start + SCREEN_TITLE_Y_OFFSET + SCREEN_LINE_Y_OFFSET + 2 * Tft.getFontYsize() + SCREEN_LINE_SPACE;

	
	line3_y = y_start + SCREEN_TITLE_Y_OFFSET + SCREEN_LINE_Y_OFFSET + 3 * Tft.getFontYsize() + 2 * SCREEN_LINE_SPACE;

	
	line4_y = y_start + SCREEN_TITLE_Y_OFFSET + SCREEN_LINE_Y_OFFSET + 4 * Tft.getFontYsize() + 3 * SCREEN_LINE_SPACE;

	
	line5_y = y_start + SCREEN_TITLE_Y_OFFSET + SCREEN_LINE_Y_OFFSET + 5 * Tft.getFontYsize() + 4 * SCREEN_LINE_SPACE;


	// set string pointers to be printed
	first_value = value_1;
	second_value = value_2;
	third_value = value_3;
	fourth_value = value_4;
	fifth_value = value_5;
		
}
void SCREEN::erase_screen()
{
/*
this method will erase the whole screen, except the arrows and the warning strip
 */
	// erase screen
	
	y_start = speed_y_size;
	x_end = Tft.getDisplayXSize() - LATERAL_BUTTON_WIDTH;
	y_end = Tft.getDisplayYSize() - WARNING_STRIP;

	
	Tft.fillScreen(x_start, x_end, y_start, y_end,BLACK);
	
}

void SCREEN::print_screen()
{
/*
This method will print the screen title and the lines, not  the values.
the line_highlight is  used to select the line, if the screen value is to be edited
*/
	unsigned int line_color = 0;
	
	erase_screen();
	//print title
	line_color = WHITE;
	Tft.drawString(title,strlen(title), CENTER, y_start+ SCREEN_TITLE_Y_OFFSET, FONT_SIZE,BLUE,BLACK);

	//print lines
	if (line_higlight == 1)
	{
		line_color = YELLOW;
	}
	else
	{
		line_color = WHITE;
	}
	Tft.drawString(line_1,0, x_start, line1_y, FONT_SIZE, line_color, BLACK);
	if (line_higlight == 2)
	{
		line_color = YELLOW;
	}
	else
	{
		line_color = WHITE;
	}
	Tft.drawString(line_2,0, x_start, line2_y, FONT_SIZE, line_color, BLACK);
	if (line_higlight == 3)
	{
		line_color = YELLOW;
	}
	else
	{
		line_color = WHITE;
	}
	Tft.drawString(line_3,0, x_start, line3_y, FONT_SIZE, line_color, BLACK);
	if (line_higlight == 4)
	{
		line_color = YELLOW;
	}
	else
	{
		line_color = WHITE;
	}
	Tft.drawString(line_4, 0, x_start, line4_y, FONT_SIZE, line_color, BLACK);
	if (line_higlight == 5)
	{
		line_color = YELLOW;
	}
	else
	{
		line_color = WHITE;
	}
	Tft.drawString(line_5, 0, x_start, line5_y, FONT_SIZE, line_color, BLACK);
}

void SCREEN::update_screen()
{/*
 This method will update the values , not the lines.
 if the value is to be edited, will be drawn in yellow
 */
	unsigned int line_color = 0;
	

	if ((line_higlight == 1) && (select_line))
	{
		line_color = YELLOW;
	}
	else
	{
		line_color = WHITE;
	}
	
	Tft.drawString(first_value, line1_x, line1_y, FONT_SIZE, line_color, BLACK);
	
	if ((line_higlight == 2) && (select_line))
	{
		line_color = YELLOW;
	}
	else
	{
		line_color = WHITE;
	}
	
	Tft.drawString(second_value, line2_x, line2_y, FONT_SIZE, line_color, BLACK);

	if ((line_higlight == 3) && (select_line))
	{
		line_color = YELLOW;
	}
	else
	{
		line_color = WHITE;
	}
	
	Tft.drawString(third_value, line3_x, line3_y, FONT_SIZE, line_color, BLACK);
	
	if ((line_higlight == 4) && (select_line))
	{
		line_color = YELLOW;
	}
	else
	{
		line_color = WHITE;
	}
	
	Tft.drawString(fourth_value, line4_x, line4_y, FONT_SIZE, line_color, BLACK);

	if ((line_higlight == 5) && (select_line))
	{
		line_color = YELLOW;
	}
	else
	{
		line_color = WHITE;
	}
	
	Tft.drawString(fifth_value, line5_x, line5_y, FONT_SIZE, line_color, BLACK);
}


void print_info_line(char info, String* string_to_display)
{
	
	unsigned int current_back_color = 0;
	//String string_to_display;
	


	if (info == CLEAR)
	{
		current_back_color = BLACK;
		Tft.fillScreen(0, Tft.getDisplayXSize() - 1, Tft.getDisplayYSize() - WARNING_STRIP, Tft.getDisplayYSize() - 1, BLACK);
	}
	else
	{
		if(info==ERROR)
		{
			
			current_back_color = RED;
		}
		else if(info==WARNING)
		{
			
			current_back_color = YELLOW;
		}
				
		Tft.fillScreen(0, Tft.getDisplayXSize() - 1, Tft.getDisplayYSize() - WARNING_STRIP, Tft.getDisplayYSize() - 1, current_back_color);
		
		Tft.drawString(string_to_display, 2, Tft.getDisplayYSize() - WARNING_STRIP +5,1,BLACK, current_back_color);	// print the first on the list
		
		
	}
				

}
void Decrement_screen_num(void)
{
	if (current_screen_num == 0)
	{
		current_screen_num = SCREEN_NUM - 1;
	}
	else
	{
		current_screen_num--;
	}
}
void Increment_screen_num(void)
{
	if (current_screen_num == SCREEN_NUM - 1)
	{
		current_screen_num = 0;
	}
	else
	{
		current_screen_num++;
	}
}
void Check_errors_number(void)
{
	/*will check if the buffer can handle all the errors that are present*/
	if (number_of_errors >= MAX_INFO_LINE)
	{
		number_of_errors = MAX_INFO_LINE - 1;
	}

}
void Check_warnings_number(void)
{
	/*will check if the buffer can handle all the errors that are present*/
	if (number_of_warnings >= MAX_INFO_LINE)
	{
		number_of_warnings = MAX_INFO_LINE - 1;
	}

}

void CheckSecretCombinationStates(void)
{
	/*this function will check and manage the secret combination. In this case, the secret combination of the joystick will be the : down-up-down,
	each movement has to be done in less than half a second. Once the combination has been done correctly ( marked as a gree arrow), 
	if the user clicks the joystick, a special config message will be sent.
	THe combination is only possible in screens with not editable parameters.
	This function is called at the same rate than check_joystick*/

	switch (security_state)
	{
	case STATE_WAITING:
		state_open_counter = 0; // reset the timer
		arrow_color = WHITE;
		if (signal_up_detected) // not valid  movement
		{
			security_state = STATE_WAITING; //go to beggining	
			signal_up_detected = FALSE;//reset flag
		}
		else if (signal_down_detected) //valid movement
		{
			security_state = STATE_STEP_1; //go to next state
			signal_down_detected = FALSE;//reset flag
		}
		else  //no signal detected, 
		{
			security_state = STATE_WAITING; //go to beggining
		}
		break;
	case STATE_STEP_1:
		arrow_color = YELLOW;
		if (signal_up_detected) //valid  movement
		{
			security_state = STATE_STEP_2; //go to next state
			signal_up_detected = FALSE;//reset flag
		}
		else if (signal_down_detected) //valid movement
		{
			security_state = STATE_WAITING; //go to beggining
			signal_down_detected = FALSE;//reset flag
		}
		else  //no signal detected, 
		{
			security_state = STATE_WAITING; //go to beggining
		}
		break;
	case STATE_STEP_2:
		arrow_color = BLUE;
		if (signal_up_detected) // not valid  movement
		{
			security_state = STATE_WAITING; //go to beggining	
			signal_up_detected = FALSE;//reset flag
		}
		else if (signal_down_detected) //valid movement
		{
			security_state = STATE_OPEN; //go to next state
			state_open_counter = 10;//set counter
			signal_down_detected = FALSE;//reset flag
		}
		else //no signal detected,
		{
			security_state = STATE_WAITING; //go to beggining
		}
		break;
	case STATE_OPEN:
		arrow_color = GREEN;
		if (state_open_counter)
		{
			state_open_counter--;
			security_state = STATE_OPEN;//stay in open state until counter reaches zero
		}
		else
		{
			security_state = STATE_WAITING; //go to beggining
		}

		break;
	default:
		security_state = STATE_WAITING; //go to beggining	
		break;

	}
	// print the right arrow to signal the progression of the combination
	Tft.drawString(right_str, RIGHT, ((Tft.getDisplayYSize()) / 2), 3, arrow_color, BLACK);
}
void CheckSecretCombinationClick(void)
{/*TBR:will check what to do when a clicck of the joystick is sensed, this is the final point of the secret combination. inserted in the isr of the clicck*/
	if (security_state == STATE_OPEN)
	{
		/*TBR.The result of a correct combination is defined here. In this case, a reconfiguring message will be sent, */
	send_config_CAN_mssg = 1;	// will send config message
	reconfigure_pedelec = 1;
	}
	else if (Flag_RB && Cntr_breaking)	//if not in the correct combination: if a click is sensed and we are not in the open state, will also send a reconfigurring message.
	{
	send_config_CAN_mssg = 1;	// will send config message
	reconfigure_pedelec = 1;
	}
}