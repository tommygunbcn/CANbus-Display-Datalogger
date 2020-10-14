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

/* define here the union structures declared in CAN_defs.h. TBR */

#include "CAN_db.h"


//---------------------------------------------------------------------------pedelec
union TX_pedelec_1 TX_pedelec_brdcast_1 = { 0 }; //broadcast message 1
union TX_pedelec_2 TX_pedelec_brdcast_2 = { 0 };
union TX_pedelec_3 TX_pedelec_brdcast_3 = { 0 };
union TX_pedelec_4 TX_pedelec_brdcast_4 = { 0 };
union TX_pedelec_6 TX_pedelec_brdcast_6 = { 0 };
union TX_CAN_PEDELEC_COMMAND TX_msg_PEDELEC_COMMAND = { 0 };
union RX_pedelec_answer RX_pedelec_brdcast_answer = { 0 };
//------------------------------------------------------------------------------BMS
union RX_CAN_BMS_analog RX_msg_BMS_analog = { 0 };
union RX_CAN_BMS_analog2 RX_msg_BMS_analog2 = { 0 };
union RX_CAN_BMS_digital RX_msg_BMS_digital = { 0 };
//-------------------------------------------------------------------------------Controller
union RX_Broadcast_0 RX_Ctl_brdcst_0 = { 0 };
union RX_Broadcast_1 RX_Ctl_brdcst_1 = { 0 };
union RX_Broadcast_2 RX_Ctl_brdcst_2 = { 0 };
union RX_Broadcast_3 RX_Ctl_brdcst_3 = { 0 };
union RX_Broadcast_4 RX_Ctl_brdcst_4 = { 0 };
union RX_Broadcast_5 RX_Ctl_brdcst_5 = { 0 };
union RX_Broadcast_6 RX_Ctl_brdcst_6 = { 0 };
union TX_CAN_CNTR_COMMAND TX_msg_CNTR_COMMAND = { 0 };;
union RX_CNTR_answer RX_CNTR_answer = { 0 };

