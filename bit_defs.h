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
In this file useful bit structures are defined
*/
#ifndef _bit_defs_h
#define _bit_defs_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
// word definitions
struct bit_word_def {
	char	b_0 : 1;
	char	b_1 : 1;
	char	b_2 : 1;
	char	b_3 : 1;
	char	b_4 : 1;
	char	b_5 : 1;
	char	b_6 : 1;
	char	b_7 : 1;
	char	b_8 : 1;
	char	b_9 : 1;
	char	b_10 : 1;
	char	b_11 : 1;
	char	b_12 : 1;
	char	b_13 : 1;
	char	b_14 : 1;
	char	b_15 : 1;
};
union word_def {
	struct bit_word_def b;
	struct {
		char    low;                /* Lower byte */
		char    high;               /* Higher byte */
	}byte;
	short   word;
};

// byte definition
struct bit_def {
	char	b_0 : 1;
	char	b_1 : 1;
	char	b_2 : 1;
	char	b_3 : 1;
	char	b_4 : 1;
	char	b_5 : 1;
	char	b_6 : 1;
	char	b_7 : 1;
};

union byte_def {
	struct bit_def b;
	char    byte;
};


#endif

