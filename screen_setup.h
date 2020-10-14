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
In this file all the screens will be defined. to add or remove a screen follow the instructions
*/


#ifndef _SCREEN_SETUP_h
#define _SCREEN_SETUP_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
//functions. To be reviewed if any change is made to CAN nodes
void check_info_line(void);  
void Update_info_line(void);
void Set_current_screen(unsigned char screen_num);

//TBR:to add or remove screens the next lines must be edited. Screens should be declared in order, from 0 to x
#define SCREEN_NUM	6	// number of screens, to be updated every time a screen is added or substracted
extern SCREEN screen_0;
extern SCREEN screen_1;
extern SCREEN screen_2;
extern SCREEN screen_3;
extern SCREEN screen_4;
extern SCREEN screen_5;


extern SCREEN current_screen;				// current screen being displayed

//if a screen doesnt have fixed text, but volatile text, declare a buffer of strings
extern String BMS_digital_str[5];

//TBR :if the screen has editable values, for example in screen 5, declare them here
extern String cntr_Ki_str;
extern String cntr_Kp_str;
extern String cntr_max_gate_time_str;
extern String cntr_bemf_trim_str;

//TBR:declare also the flags that will mark wich line will be edited
extern bool change_ki;
extern bool change_kp;
extern bool change_max_gate_var;
extern bool change_bemf_trim;


/*TBR*/
/*for every error word or error byte that is used (defined in variables.h), a complementary error/byte has to be defined here
same kind of variable. ex: ped_error_word_1------------->ped_error_word_1_prev */
extern unsigned int ped_error_word_1_prev;		// used to detect a change in the error word
extern unsigned int cnt_error1_word_prev;	// used to detect a change in the error word 1 of the controller
extern unsigned int cnt_error2_word_prev;	// used to detect a change in the error word 2 of the controller
extern unsigned int cnt_WARNING_word_1_prev;// used to detect a change in the warning word
/*define here the special flags*/
extern volatile bool analog_2_received; // flag used to detect if a certain message has arrived
/*define the flags used to detect if a node is not sending messages. The detection of a node timeout is made in the timer interruption*/
//ped
extern volatile bool Flag_ped_can_wdt;
extern volatile bool Flag_no_pedelec_can;  //Flag set if no pedelec node is present in the system.will be set as long as no can message arrives from this node
extern volatile bool Flag_no_pedelec_can_prev ;	// used to detect a change in the flag
//bms
extern volatile bool Flag_bms_can_wdt;
extern volatile bool Flag_no_bms_can;
extern volatile bool Flag_no_bms_can_prev;		// used to detect a change in the flag
//controller
extern volatile bool Flag_controller_can_wdt;
extern volatile bool Flag_no_controller_can;
extern volatile bool Flag_no_controller_can_prev; // used to detect a change in the flag
//for every flag defined, a warning message has to be defined
extern const  char no_pedelec[];
extern const  char no_controller[];
extern const  char no_bms[];
#endif