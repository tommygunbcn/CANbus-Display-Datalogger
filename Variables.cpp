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
In this file all the variables from each node shall be defined. TBR
*/

#include "Global.h"

//----------------------------------------------------Analog read values
//current sensor

int max_current_ARMS = 100;//maximum current arms that the sensor can sense
int current_read_value = 0;
float ph_current_measured_arms = 0;

//--------------------------------------------------------------------------------------------- Pedelec node values
float Thrott_supply_volt = 0;
float Power_supply_volt = 0;
float throttle_input = 0;
float throttle_output = 0;
float ped_counter = 0;

unsigned int odometer;
unsigned int working_hours;
// param values
byte_def config_byte;
unsigned char max_speed = 0;
unsigned char pedal_events = 0;
float no_pedal_thr = 0;
float max_pedal_thr = 0;
bool debug_state = 0;
//configuration parameters
union Start_params Start_parameters = { 0 };// paremeters of the bike received
//error word
volatile union  word_def ped_ERROR_Word_1 = { 0x0000 };
//warning byte
volatile union byte_def ped_warning_byte = { 0x00 };
volatile union byte_def ped_info_byte = { 0x00 };
// error strings
/*declared this way so that the program memory is used , and not the RAM memory .bear in mind the lenght!!*/
const  char ped_can_error[] PROGMEM = { "CAN ERROR" };
const  char ped_sw_error[] PROGMEM = { "SW ERROR" };
const  char ped_thr_in_error[] PROGMEM = { "THOTTLE INPUT" };
const  char ped_thr_sup_1_error[] PROGMEM = { "THOTTLE SUPP. OVER" };
const  char ped_thr_sup_2_error[] PROGMEM = { "THOTTLE SUPP. UNDER" };
const  char ped_can_tx_error[] PROGMEM = { "CAN OVERLOAD" };
const  char ped_no_bms_can_error[] PROGMEM = { "NO BMS" };
const  char ped_bms_dig_error[] PROGMEM = { "BMS DIGITAL" };
const  char ped_no_contr_can_error[] PROGMEM = { "NO CONTROLLER CAN" };//{ "NO CONTROLLER CAN MESSAGE" };
const  char ped_DAC_error[] PROGMEM = { "DAC ERROR" };//{ "PEDELEC DAC ERROR" }
const  char ped_supply_over_error[] PROGMEM = { "POWER SUPPLY OVERVOLTAGE" };//{ "POWER SUPPLY OVERVOLTAGE" };
const  char ped_supply_under_error[] PROGMEM = { "POWER SUPPLY UNDERVOLTAGE" };//{ "POWER SUPPLY UNDERVOLTAGE" };
const  char ped_blank_flash_error[] PROGMEM = { "BLANK FLASH ERROR" };//{ "BLANK FLASH ERROR" };
const  char ped_memory_error[] PROGMEM = { "MEMORY READING ERROR" };//{ "MEMORY READING ERROR" };
//---------------------------------------------------------------------------------------------------------------------------------------------BMS values
float BMS_voltage = 0;
float BMS_current = 0;
unsigned int BMS_temp = 0;
unsigned int BMS_soc = 0;
unsigned int BMS_cycles = 0;
//the error word/byte , if present, must be defined here
volatile union  word_def BMS_digital_word = { 0x0000 };
// error strings
/*declared this way so that the program memory is used , and not the RAM memory .bear in mind the lenght!!*/
const  char bms_charging[] PROGMEM = { "Batt.charging!" };	// 
const  char bms_overtemp[] PROGMEM = { "Batt Overtemp" };
const  char bms_overcurr[] PROGMEM = { "Batt.Overcurr." };
const  char bms_cell_imbalance[] PROGMEM = { "Cell imbalance" };
const  char bms_cell_oor[] PROGMEM = { "Cell out range" };
const  char bms_overvoltage[] PROGMEM = { "Overvoltage" };
const  char bms_undervoltage[] PROGMEM = { "Undervoltage" };
const  char bms_com_fault[] PROGMEM = { "BMS com.fault" };
const  char bms_lost_comm[] PROGMEM = { "BMS no comm." };
//------------------------------------------------------------------------------------------------------------------------------------------Controller values

float Cntr_voltage = 0;
//unsigned int Mot_temp = 0;
//unsigned int Cntr_temp = 0;
float Cntr_current = 0;
unsigned char Cntr_breaking = 0;

float Cntr_ph_current_req = 0;
float Cntr_ph_current = 0;
//signed int Cntr_pi_res = 0;
//unsigned char Cntr_mod_index = 0;
float Cntr_power_input = 0;

unsigned int speed_int = 0;
//unsigned long Cntr_odo = 0;
//unsigned int Cntr_trip = 0;
unsigned int Cntr_usage = 0;
//general params value
unsigned char contr_version_high = 0;
unsigned char contr_version_low = 0;
bool beta_version = 0;
//pi param values
unsigned char ki = 0;
unsigned char kp = 0;
unsigned char max_gate_var = 0;
unsigned char bemf_trim = 0;
//configuration  parameters, 
union Pi_params Pi_parameters = { 0 };	// pi of the controller
union Gen_params general_parameters = { 0 };	// pedelec
//the error word/byte , if present, must be defined here
volatile union  word_def cnt_ERROR_Word_1 = { 0x0000 };
volatile union  word_def cnt_ERROR_Word_2 = { 0x0000 };
volatile union  word_def cnt_WARNING_Word_1 = { 0x0000 };
//controller error
// error strings
/*declared this way so that the program memory is used , and not the RAM memory .bear in mind the lenght!!
cnt_error[] will be used to concatenate the strings, making clear the error comes from the controller*/
const  char cnt_error[] PROGMEM = { ":CONTR" };	// used to concatenate

const  char cnt_Short_circuit_hw[] PROGMEM = { "SHORTCIRC. HW" };
const  char cnt_throttle_OL[] PROGMEM = { "THROTTLE OL" };
const  char cnt_throttle_SC[] PROGMEM = { "THROTTLE SC" };
const  char cnt_motor_overtemp[] PROGMEM = { "MOT. OVERTEMP" };
const  char cnt_motor_temp_err[] PROGMEM = { "MOT. NTC ERR" };
const  char cnt_contr_overtemp[] PROGMEM = { "CNTR. OVERTEMP" };
const  char cnt_contr_temp_err[] PROGMEM = { "CNTR. NTC ERR" };
const  char cnt_batt_overtemp[] PROGMEM = { "BATT. OVERTEMP" };
const  char cnt_OverVolt[] PROGMEM = { "OVERVOLT." };
const  char cnt_UnderVolt[] PROGMEM = { "UNDERVOLT." };
const  char cnt_Short_circuit[] PROGMEM = { "SHORTCIRC. SW" };
const  char cnt_DC_overcurrent[] PROGMEM = { "OVERCURRENT" };
const  char cnt_WDT[] PROGMEM = { "WDT" };
const  char cnt_Low_5v[] PROGMEM = { "LOW 5V" };
const  char cnt_Clk_Stop[] PROGMEM = { "CLK STOP" };
const  char cnt_sw_overflow[] PROGMEM = { "SW OVERFLOW" };
const  char cnt_und_instr[] PROGMEM = { "UND. INSTRUCTION" };
const  char cnt_CAN_busoff[] PROGMEM = { "CAN BUSOFF" };
const  char cnt_Hall_seq[] PROGMEM = { "HALL SEQ." };
const  char cnt_Hall_error[] PROGMEM = { "HALL ERROR" };
const  char cnt_blk_timeout[] PROGMEM = { "BLK TIEMOUT" };
const  char cnt_start_timeout[] PROGMEM = { "BKM START TIMEOUT" };
const  char cnt_BMS_overtemp[] PROGMEM = { "BMS OVERTEMP" };
const  char cnt_BMS_overcurr[] PROGMEM = { "BMS OVERCURR" };
const  char cnt_BMS_overvolt[] PROGMEM = { "BMS OVERVOLT" };
const  char cnt_BMS_undervolt[] PROGMEM = { "BMS UNDERVOLT" };
const  char cnt_Mot_e2p_error[] PROGMEM = { "E2P ERROR" };
const  char cnt_soft_error[] PROGMEM = { "MAIN ERROR" };

//warning strings
const char warning[] PROGMEM = { "WARNING:" };

const char warning_mot_temp[] PROGMEM = { "HIGH MOTOR TEMP" };
const char warning_contr_temp[] PROGMEM = { "HIGH CONTROLLER TEMP" };
const char warning_bms_temp[] PROGMEM = { "HIGH BATTERY TEMP" };
const char warning_Low_soc[] PROGMEM = { "LOW SoC" };
const char warning_DC_current[] PROGMEM = { "HIGH BATTERY CURRENT" };
const char warning_LV[] PROGMEM = { "LOW BATT VOLTAGE" };
const char warning_CAN_passive[] PROGMEM = { "CAN PASSIVE" };
const char warning_CAN_tx[] PROGMEM = { "CAN TX problem" };
const char warning_CAN_rx[] PROGMEM = { "CAN RX problem" };


//not static strings

char Speed_str[5];
//bms
char Serial_BMS_Volt_str[7];
char Display_BMS_Volt_str[7];
char Serial_BMS_curr_str[7];  // to be sent via serial
char Display_BMS_curr_str[7]; //to be displayed
char BMS_temp_str[5];
char BMS_soc_str[5];
char BMS_cycles_str[6];

//--cntr
char Serial_Cntr_Volt_str[7];  // to be sent via serial
char Display_Cntr_Volt_str[7]; //to be displayed
char Serial_Cntr_curr_str[7];  // to be sent via serial
char Display_Cntr_curr_str[7]; //to be displayed
char Mot_temp_str[5];
char Cntr_temp_str[5];
char Cntr_ph_current_req_str[5];
char Cntr_ph_current_str[5];
char ph_current_measured_arms_str[5];
char cnt_ERROR_Word_1_str[7];
char cnt_ERROR_Word_2_str[7];
char cnt_WARNING_Word_1_str[7];
char Cntr_breaking_str[3];
char Cntr_mod_index_str[5];
char Driving_mode_str[3];
char Cntr_trip_str[5];
//functions
/*the update parameters function must be created for all the editable parameters*/
void update_pi_params(void)
{
	/*populate the strings with the values updated*/
	ki = Pi_parameters.signal.ki;
	dtostrf(ki, 2, 0, received_number);
	cntr_Ki_str = (String)received_number;

	kp = Pi_parameters.signal.kp;
	dtostrf(kp, 2, 0, received_number);
	cntr_Kp_str = (String)received_number;

	max_gate_var = Pi_parameters.signal.max_gate_var;
	dtostrf(max_gate_var, 2, 0, received_number);
	cntr_max_gate_time_str = (String)received_number;

	bemf_trim = Pi_parameters.signal.bemf_trim;
	dtostrf(bemf_trim, 2, 0, received_number);
	cntr_bemf_trim_str = (String)received_number;

	if (current_screen_num == 5)		// /*same screen the editable parameters are*/
	{
		// set flags
		select_line = 0; // reset flag so that line gets deslected, to signal the user the operation was succesful
		Flag_refesh_screen = TRUE;  // set flag so that the screen will be updated
	}

}