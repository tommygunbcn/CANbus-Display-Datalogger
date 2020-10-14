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
In this file all the variables and functions afecting the joystick shall be declared
*/



#ifndef _Joystick_h
#define _Joystick_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//define the pin connections

#define  pinJoyX  A1  //analog pins
#define  pinJoyY  A0
#define  pinJoyButton 2 //digital pin

#define RIGHT_BOUNDARY	1000	// above this value will consider a right move, [adc counts]
#define LEFT_BOUNDARY	30		//below this value will consider a left move [adc counts]
#define UP_BOUNDARY	    30			// above this value will consider a up move [adc counts]
#define DOWN_BOUNDARY	1000	//below this value will consider a down move [adc counts]

extern int Xvalue;
extern int Yvalue;

extern bool signal_right_detected;
extern bool signal_left_detected;
extern bool signal_up_detected;
extern bool signal_down_detected;
extern bool signal_button_pressed;

//functions
void JoystickPinIsr(void);
void check_joystick(void);
bool CheckEditableScreen(void);

#endif

