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

/*this file defines how the screen will look like, this can be trimmed via defines. Also, the class screen is declared. Should not be affected by adding/dleting nodes or changing the database */

#ifndef _SCREEN_DEFS_h
#define _SCREEN_DEFS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif


//functions

void print_info_line(char info, String* string_to_display);
void Decrement_screen_num(void);
void Increment_screen_num(void);
void Check_errors_number(void);
void Check_warnings_number(void);
void CheckSecretCombinationStates(void);
void CheckSecretCombinationClick(void);

//defines
#define CLEAR	0
#define ERROR	1
#define WARNING	2

#define FONT_SIZE				2					 // size of normal font
#define FONT_SIZE_PIXELS		(FONT_X+FONT_SPACE)		
#define SPEED_SIZE				6					// the size of the font for the speed
#define SPEED_Y_OFFSET			10					//in pixels, the upper distance from the top to print  
#define SCREEN_TITLE_Y_OFFSET			10			//in pixels, the upper distance to print title
#define SCREEN_LINE_Y_OFFSET			15			//in pixels, the upper distance from title
#define SCREEN_LINE_SPACE             15			 // in px, distance from line to line
#define LATERAL_BUTTON_WIDTH   25					 // in pixels
#define WARNING_STRIP			20					//in pixels, the width of the warning messaages
#define MAX_INFO_LINE 5								// maximum number of messages to be displayed in the info line

extern unsigned short speed_color;				// the color of the speed shown in the display
extern unsigned char current_info_display;		// the screen number currently being displayed
extern unsigned char speed_y_size;				//
extern unsigned char current_screen_num;
//character strings program  memory
extern const char On_str[];
extern const char Off_str[];
extern const char space_str[];
extern const char release_str[];
extern const char beta_str[];
extern const char end_str[];
//character strings in RAM
extern char right_str[];
extern char left_str[];

extern unsigned int y_start, x_end, y_end;
extern unsigned int x_start;
extern unsigned int line1_y, line2_y, line3_y, line4_y, line5_y;

extern String no_str;

extern String line_1_str;
extern String line_2_str;
extern String line_3_str;
extern String line_4_str;
extern String line_5_str;

// this is the buffer of the messaged shown in the info line, in case there are more than one, they will be displayed sequentially
extern String Display[MAX_INFO_LINE];

extern volatile unsigned int number_of_errors;  // number of errors to be displayed in the warning strip
extern volatile unsigned int number_of_warnings;// number of warnings to be displayed in the warning strip
extern unsigned char line_higlight; //If a value on the screen is to be edited, it will be highlighted first. This variable will have the number of the line highlighted
extern volatile bool select_line;//select line to edit
extern bool send_config_CAN_mssg; //this flag will be set when a configuring CAN message is pending to be sent

//secret combination states
#define STATE_WAITING   0   // normal state
#define STATE_STEP_1	1
#define STATE_STEP_2	2
#define STATE_OPEN		3    // open state
extern byte   security_state;	// to be able to change parameteres
extern char   state_open_counter;  // counter to determine the time the security state remanins in open state
extern unsigned short arrow_color; // the arrow color to signal the secret combination



class SCREEN
{
	public:

		unsigned char num=0;
		
		//lines
		unsigned int line1_x, line2_x, line3_x, line4_x, line5_x;		// to print values
		unsigned int line1_y, line2_y, line3_y, line4_y, line5_y;
		
		//String title = { " " };
		const char* title;
		const char* line_1;
		const char* line_2;
		const char* line_3;
		const char* line_4;
		const char* line_5;
	
		String* first_value; //pointer to first value
		String* second_value; //pointer to second value
		String* third_value; 
		String* fourth_value; 
		String* fifth_value; 

		SCREEN();
		SCREEN(unsigned char number,const char* title , const char* line1, const char* line2, const char* line3, const char* line4, const char* line5,
				String* value_1, String* value_2,String* value_3, String* value_4, String* value_5);
		void print_screen(void);
		void erase_screen(void);
		void update_screen(void);
};

#endif