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
In this file all the screens will be defined. TBR if adding more nodes
*/
#include "global.h"



//-----------------------------------------------------------variables--------------------------

// screen 0 titles
const  char screen_0_0[] PROGMEM = { "BMS INFO" };
const  char screen_0_1[] PROGMEM = { "Voltage[V]:" };
const  char screen_0_2[] PROGMEM = { "Current[A]:" };
const  char screen_0_3[] PROGMEM = { "Temp[C]:" };
const  char screen_0_4[] PROGMEM = { "SoC[%]:" };
const  char screen_0_5[] PROGMEM = { "Cycles:" };

// screen 1 titles
const  char screen_1_0[] PROGMEM = { "CONTROLER INFO" };
const  char screen_1_1[] PROGMEM = { "Voltage[V]:" };
const  char screen_1_2[] PROGMEM = { "Motor temp[C]:" };
const  char screen_1_3[] PROGMEM = { "Cntr. temp[C]:" };
const  char screen_1_4[] PROGMEM = { "Current[A]:" };
const  char screen_1_5[] PROGMEM = { "Brake:" };

// screen 2 titles
const  char screen_2_0[] PROGMEM = { "CONTROLER INFO 2" };
const  char screen_2_1[] PROGMEM = { "ph.curr req[A]:" };
const  char screen_2_2[] PROGMEM = { "ph current[A]:" };
const  char screen_2_3[] PROGMEM = { "ph meas[Arms]:" };
const  char screen_2_4[] PROGMEM = { "mod. index[%]:" };
const  char screen_2_5[] PROGMEM = { "power input[kw]:" };

// screen 3 titles
const  char screen_3_0[] PROGMEM = { "CONTROLER INFO 3" };
const  char screen_3_1[] PROGMEM = { "odo[km]:" };
const  char screen_3_2[] PROGMEM = { "trip[km]:" };
const  char screen_3_3[] PROGMEM = { "usage[h]:" };
const  char screen_3_4[] PROGMEM = { "version:" };
const  char screen_3_5[] PROGMEM = { "ped Mot. Stop:" };


// screen 4 titles
const  char screen_4_0[] PROGMEM = { "BMS FLAGS" };
const  char screen_4_1[] PROGMEM = { "" };
//if a screen doesnt have fixed text, but volatile text, declare a buffer of strings
String BMS_digital_str[5] = { "","","","","" };

//screen 5 titles, special screen since its lines will be editable
const  char screen_5_0[] PROGMEM = { "PI PARAMETERS" };
const  char screen_5_1[] PROGMEM = { "Ki:" };
const  char screen_5_2[] PROGMEM = { "Kp:" };
const  char screen_5_3[] PROGMEM = { "max gate cnts:" };
const  char screen_5_4[] PROGMEM = { "Bemf trim cnts:" };
const  char screen_5_5[] PROGMEM = { ":" };
//define here the lines that will be editable
String cntr_Ki_str = "";
String cntr_Kp_str = "";
String cntr_max_gate_time_str = "";
String cntr_bemf_trim_str = "";
//define also the flags that will mark wich line will be edited
bool change_ki = 0;
bool change_kp = 0;
bool change_max_gate_var = 0;
bool change_bemf_trim = 0;


// definition of the screens used as they are declared in screen_setup.h

SCREEN		screen_0(0, screen_0_0, screen_0_1, screen_0_2, screen_0_3, screen_0_4, screen_0_5,
	&line_1_str, &line_2_str, &line_3_str, &line_4_str, &line_5_str);

SCREEN		screen_1(1, screen_1_0, screen_1_1, screen_1_2, screen_1_3, screen_1_4, screen_1_5,
	&line_1_str, &line_2_str, &line_3_str, &line_4_str, &line_5_str);


SCREEN		screen_2(2, screen_2_0, screen_2_1, screen_2_2, screen_2_3, screen_2_4, screen_2_5,
	&line_1_str, &line_2_str, &line_3_str, &line_4_str, &line_5_str);


SCREEN		screen_3(3, screen_3_0, screen_3_1, screen_3_2, screen_3_3, screen_3_4, screen_3_5,
	&line_1_str, &line_2_str, &line_3_str, &line_4_str, &line_5_str);
/*this screen is a little bit different, as it has no fis¡xed text. Notice that another buffer is used (BMS_digital_str)*/
SCREEN		screen_4(4, screen_4_0, screen_4_1, screen_4_1, screen_4_1, screen_4_1, screen_4_1,

	&BMS_digital_str[0], &BMS_digital_str[1], &BMS_digital_str[2], &BMS_digital_str[3], &BMS_digital_str[4]);

SCREEN		screen_5(5, screen_5_0, screen_5_1, screen_5_2, screen_5_3, screen_5_4, screen_5_5,
/*this screen is also a bit special, as the parameters are editable, the strings must be different that line_1_str, etc...*/
	&cntr_Ki_str, &cntr_Kp_str, &cntr_max_gate_time_str, &cntr_bemf_trim_str, &no_str);

//instantiate objects
SCREEN current_screen;				// current screen being displayed

/*for every error word or error byte that is used (defined in variables.h), a complementary error/byte has to be defined here
same kind of variable. ex: ped_error_word_1------------->ped_error_word_1_prev */
unsigned int ped_error_word_1_prev = 0;		// used to detect a change in the error word
unsigned int cnt_error1_word_prev = 0;	// used to detect a change in the error word 1 of the controller
unsigned int cnt_error2_word_prev = 0;	// used to detect a change in the error word 2 of the controller
unsigned int cnt_WARNING_word_1_prev = 0;// used to detect a change in the warning word

/*define here the special flags*/
volatile bool analog_2_received = FALSE; // flag used to detect if a certain message has arrived
/*define the flags used to detect if a node is not sending messages*/
//ped
volatile bool Flag_ped_can_wdt = 0;			// auxiliary flag
volatile bool Flag_no_pedelec_can = 0;    //Flag set if no pedelec node is present in the system.will be set as long as no can message arrives from this node
volatile bool Flag_no_pedelec_can_prev = 0;	// used to detect a change in the flag
//bms
volatile bool Flag_bms_can_wdt = 0;
volatile bool Flag_no_bms_can = 0;
volatile bool Flag_no_bms_can_prev = 0;		// used to detect a change in the flag
//controller
volatile bool Flag_controller_can_wdt = 0;
volatile bool Flag_no_controller_can = 0;
volatile bool Flag_no_controller_can_prev = 0; // used to detect a change in the flag


//for every flag defined, a warning message has to be defined
const  char no_pedelec[] PROGMEM = { "NO PEDELEC CAN MESSAGE" };
const  char no_controller[] PROGMEM = { "NO CONTROLLER CAN MESSAGE" };
const  char no_bms[] PROGMEM = { "NO BMS CAN MESSAGE" };






//------------------------------------------------------------------------------------functions---------------------------------------
void check_info_line(void)
{/*
 This function is independent to the screen being displayed, will detect if some error or warning flag is set, and will feed the messages buffer with the correct strings.
 error situation have preference to  warning situations. Will also erase the strip if no error and no warnings present.
 This checking will be made at the same rate of screen refreshing
 Must be revised if any error flag or word is modified
 */
	char buffer[35];
	char buffer_aux[35];

	// first find out if there is any error present or timeout receiving messages
	if ((ped_ERROR_Word_1.word) || (Flag_no_pedelec_can) || (Flag_no_bms_can) || (Flag_no_controller_can) ||
		(cnt_ERROR_Word_1.word) || (cnt_ERROR_Word_2.word))
	{
		// if any error present, check if there is any change from the last iteration
		if ((ped_ERROR_Word_1.word != ped_error_word_1_prev)
			|| (cnt_ERROR_Word_1.word != cnt_error1_word_prev)
			|| (cnt_ERROR_Word_2.word != cnt_error2_word_prev)
			|| (Flag_no_pedelec_can != Flag_no_pedelec_can_prev)
			|| (Flag_no_bms_can != Flag_no_bms_can_prev)
			|| (Flag_no_controller_can != Flag_no_controller_can_prev))
		{
			// new error situation, check one by one the possible errors. Note that the error buffer will be populated one after the other. The error checked first will have priority. once
			// the buffer is full , the error remaining are not shown.
			number_of_errors = 0;		// reset number of errors

			//--------------------------------------------------------------------------------------------------------checks the pedelec errors
			if (Flag_can_error)
			{
				// increment the error counter
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				//copy the string in program memory, to the buffer in RAM
				strcpy_P(buffer, (ped_can_error));
				//add the string to the Display buffer
				Display[number_of_errors - 1] = (String)buffer;

			}
			if (Flag_main_task_error)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (ped_sw_error));

				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_throttle_error)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (ped_thr_in_error));

				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_thrtt_supply_over)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (ped_thr_sup_1_error));

				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_thrtt_supply_under)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (ped_thr_sup_2_error));
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_can_tx_error)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (ped_can_tx_error));
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_no_bms_can_error)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (ped_no_bms_can_error));
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_bms_digital_error)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (ped_bms_dig_error));
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_no_contr_can_error)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (ped_no_contr_can_error));
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_contr_alarm_error)
			{
				/*no used anymore*/
			}

			if (Flag_DAC_error)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (ped_DAC_error));
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_pw_supply_over)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (ped_supply_over_error));
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_pw_supply_under)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (ped_supply_under_error));
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_blank_flash_error)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (ped_blank_flash_error));
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_flash_mem_error)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (ped_memory_error));
				Display[number_of_errors - 1] = (String)buffer;
			}
			//-----------------------------------------------------------------------------------------------------controller-error-1-----------------------------------------
			strcpy_P(buffer_aux, cnt_error);// to concatenate
			if (Flag_Short_circuit_hw)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_Short_circuit_hw));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_throttle_OL)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_throttle_OL));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_throttle_SC)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_throttle_SC));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_motor_overtemp)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_motor_overtemp));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_motor_temp_err)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_motor_temp_err));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_contr_overtemp)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_contr_overtemp));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_contr_temp_err)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_contr_temp_err));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_batt_overtemp)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_batt_overtemp));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_OverVolt)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_OverVolt));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_UnderVolt)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_UnderVolt));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_Short_circuit)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_Short_circuit));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_DC_overcurrent)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_DC_overcurrent));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_WDT)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_WDT));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_Low_5v)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_Low_5v));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_Clk_Stop)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_Clk_Stop));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_sw_overflow)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_sw_overflow));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			//------------------------------------------------------------------------------------------------------------------controller-error-2-----------------------------------------

			if (Flag_und_instr)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_und_instr));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_CAN_busoff)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_CAN_busoff));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_Hall_seq)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_Hall_seq));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_Hall_error)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_Hall_error));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_blk_timeout)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_blk_timeout));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_BMS_overtemp)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_BMS_overtemp));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_BMS_overcurr)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_BMS_overcurr));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_BMS_overvolt)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_BMS_overvolt));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_BMS_undervolt)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_BMS_undervolt));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_Mot_e2p_error)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_Mot_e2p_error));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_soft_error)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (cnt_soft_error));
				strcat(buffer, buffer_aux);
				Display[number_of_errors - 1] = (String)buffer;
			}

			
			//---------------------------------------------------------------------------------------------------------------------lack of messaging
			if (Flag_no_pedelec_can)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (no_pedelec));
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_no_bms_can)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (no_bms));
				Display[number_of_errors - 1] = (String)buffer;
			}
			if (Flag_no_controller_can)
			{
				number_of_errors++;
				//check for the maximum error situation.
				Check_errors_number();
				strcpy_P(buffer, (no_controller));
				Display[number_of_errors - 1] = (String)buffer;
			}


			// print the first string inmediately
			print_info_line(ERROR, &Display[0]);

		}
		else {
				/*do nothing, no changes in the error situation, leave the display buffer as is*/
			}
	}
	//-----------------------------------------------------------------------------------------------------------------------------------warnings
	/*no errors at all, check if some warning*/
	else if (cnt_WARNING_Word_1.word)
	{
		if (cnt_WARNING_Word_1.word != cnt_WARNING_word_1_prev)		// new warning situation
		{
			number_of_warnings = 0;		// reset number of errors
			strcpy_P(buffer, (warning)); //used to concatenate: warning: ..etc..
			if (Flag_motor_temp_lim)
			{
				number_of_warnings++;
				Check_warnings_number();
				strcpy_P(buffer_aux, (warning_mot_temp));
				strcat(buffer, buffer_aux);
				Display[number_of_warnings - 1] = (String)buffer;

			}
			if (Flag_cont_temp_lim)
			{
				number_of_warnings++;
				Check_warnings_number();
				strcpy_P(buffer, (warning_contr_temp));
				strcat(buffer, buffer_aux);
				Display[number_of_warnings - 1] = (String)buffer;

			}
			if (Flag_batt_temp_lim)
			{
				number_of_warnings++;
				Check_warnings_number();
				strcpy_P(buffer, (warning_bms_temp));
				strcat(buffer, buffer_aux);
				Display[number_of_warnings - 1] = (String)buffer;

			}
			if (Flag_batt_soc_lim)
			{
				number_of_warnings++;
				Check_warnings_number();
				strcpy_P(buffer, (warning_Low_soc));
				strcat(buffer, buffer_aux);
				Display[number_of_warnings - 1] = (String)buffer;

			}
			if (Flag_DC_lim)
			{
				number_of_warnings++;
				Check_warnings_number();
				strcpy_P(buffer, (warning_DC_current));
				strcat(buffer, buffer_aux);
				Display[number_of_warnings - 1] = (String)buffer;

			}
			if (Flag_DC_lv_lim)
			{
				number_of_warnings++;
				Check_warnings_number();
				strcpy_P(buffer, (warning_LV));
				strcat(buffer, buffer_aux);
				Display[number_of_warnings - 1] = (String)buffer;

			}
			if (Flag_CAN_Passive)
			{
				number_of_warnings++;
				Check_warnings_number();
				strcpy_P(buffer, (warning_CAN_passive));
				strcat(buffer, buffer_aux);
				Display[number_of_warnings - 1] = (String)buffer;

			}
			if (Flag_CAN_Tx)
			{
				number_of_warnings++;
				Check_warnings_number();
				strcpy_P(buffer, (warning_CAN_tx));
				strcat(buffer, buffer_aux);
				Display[number_of_warnings - 1] = (String)buffer;

			}
			if (Flag_CAN_Rx)
			{
				number_of_warnings++;
				Check_warnings_number();
				strcpy_P(buffer, (warning_CAN_rx));
				strcat(buffer, buffer_aux);
				Display[number_of_warnings - 1] = (String)buffer;

			}
			if (Flag_CAN_Rx)
			{
				number_of_warnings++;
				Check_warnings_number();
				strcpy_P(buffer, (warning_CAN_rx));
				strcat(buffer, buffer_aux);
				Display[number_of_warnings - 1] = (String)buffer;

			}

			print_info_line(WARNING, &Display[0]); // print the first warning
		}
		else
		{
			/*do nothing*/
		}

	}

	else   //no errors and no warnings, 
	{
	
			//set current error/warning display to zero for the next warning situation
			current_info_display = 0;
			//reset counters
			number_of_errors = 0;
			number_of_warnings = 0;
			// clear strip
			print_info_line(CLEAR, &no_str);
	

	}

	// update the error and warning words for comparison next loop.
	ped_error_word_1_prev = ped_ERROR_Word_1.word;
	cnt_error1_word_prev = cnt_ERROR_Word_1.word;
	cnt_error2_word_prev = cnt_ERROR_Word_2.word;
	cnt_WARNING_word_1_prev = cnt_WARNING_Word_1.word;

	Flag_no_pedelec_can_prev = Flag_no_pedelec_can;
	Flag_no_bms_can_prev = Flag_no_bms_can;
	Flag_no_controller_can_prev = Flag_no_controller_can;
}


void Update_info_line(void)
{
	/*update the info line if its needed, if there are more than one error flags active. will toggle the errors*/
	// check if any error is active. 
	if ((ped_ERROR_Word_1.word) || (Flag_no_pedelec_can) || (Flag_no_bms_can) || (Flag_no_controller_can) ||
		(cnt_ERROR_Word_1.word) || (cnt_ERROR_Word_2.word))
	{
		if (number_of_errors > 1) // if there is only one error, no need to print again
		{
			current_info_display++; //update display
			if (current_info_display >= number_of_errors)
			{
				current_info_display = 0;
			}

			print_info_line(ERROR, &Display[current_info_display]);

		}
	}
	// warnings are secondary
	else if (number_of_warnings > 1)
	{
		current_info_display++;
		if (current_info_display >= number_of_warnings)
		{
			current_info_display = 0;
		}

		print_info_line(WARNING, &Display[current_info_display]);

	}


}

void Set_current_screen(unsigned char screen_num)
{
	/*to be edited every time a screen is added or removed. It a CAN message timeout, the screens will be erased*/
	switch (screen_num)
	{
	case 0:												// bms screen
		//since parameters not editables, set these values to zero
		select_line = 0;
		line_higlight = 0;
		// pepare the strings
		if (Flag_no_bms_can)   // in case of no CAN messages, set everything blank
		{
			strcpy_P(received_number, (space_str));
			line_1_str = received_number;
			line_2_str = received_number;
			line_3_str = received_number;
			line_4_str = received_number;
			line_5_str = received_number;
		}
		else
		{
			line_1_str = Display_BMS_Volt_str;
			line_2_str = Display_BMS_curr_str;
			line_3_str = BMS_temp_str;
			line_4_str = BMS_soc_str;
			line_5_str = BMS_cycles_str;
		}

		current_screen = screen_0;
		break;
	case 1:						// controller screen
		select_line = 0;
		line_higlight = 0;
		if (Flag_no_controller_can)
		{
			strcpy_P(received_number, (space_str));
			line_1_str = received_number;
			line_2_str = received_number;
			line_3_str = received_number;
			line_4_str = received_number;
			line_5_str = received_number;
		}
		else
		{
			line_1_str = Display_Cntr_Volt_str;
			line_2_str = Mot_temp_str;
			line_3_str = Cntr_temp_str;
			line_4_str = Display_Cntr_curr_str;      
			if (Cntr_breaking)
			{
				strcpy_P(received_number, (On_str));
			}
			else
			{
				strcpy_P(received_number, (Off_str));
			}

			line_5_str = received_number;  // not a number but a text
		}


		current_screen = screen_1;
		break;
	case 2:
		select_line = 0;
		line_higlight = 0;
		if (Flag_no_controller_can)
		{
			strcpy_P(received_number, (space_str));
			line_1_str = received_number;
			line_2_str = received_number;
			line_3_str = received_number;
			line_4_str = received_number;
			line_5_str = received_number;
		}
		else
		{
			line_1_str = Cntr_ph_current_req_str;
			line_2_str = Cntr_ph_current_str;
			line_3_str = ph_current_measured_arms;
			line_4_str = Cntr_mod_index_str;
			dtostrf(Cntr_power_input, 3, 2, received_number);
			line_5_str = received_number;
		}

		current_screen = screen_2;

		break;
	case 3:
		select_line = 0;
		line_higlight = 0;
		if (Flag_no_controller_can)
		{
			strcpy_P(received_number, (space_str));
			line_1_str = received_number;
			line_2_str = received_number;
			line_3_str = received_number;
			line_4_str = received_number;
			line_5_str = received_number;
		}
		else
		{
			dtostrf(RX_Ctl_brdcst_0.signal.Mileage, 5, 0, received_number);
			line_1_str = received_number;

			//dtostrf(Cntr_trip, 3, 0, received_number);
			line_2_str = Cntr_trip_str;

			dtostrf(RX_Ctl_brdcst_0.signal.working_hours, 4, 0, received_number);
			line_3_str = received_number;

			// a bit special since is text and not a number
			dtostrf(contr_version_high, 1, 0, received_number_aux);
			strcat(received_number_aux, "v");
			dtostrf(contr_version_low, 2, 0, received_number);
			strcat(received_number_aux, received_number);
			if (beta_version)
			{
				strcpy_P(received_number_aux_2, beta_str);
			}
			else
			{
				strcpy_P(received_number_aux_2, release_str);
			}
			strcat(received_number_aux, received_number_aux_2);
			line_4_str = received_number_aux;


			if (Flag_breaking)
			{
				strcpy_P(received_number, (On_str));
			}
			else
			{
				strcpy_P(received_number, (Off_str));
			}
			line_5_str = received_number;
		}

		current_screen = screen_3;
		break;
	case 4:    //screen with variable lines


		select_line = 0;
		line_higlight = 0;
		current_screen = screen_4;
		break;
	case 5:  // screen with editable parameters

		current_screen = screen_5;

		break;


	}
}
