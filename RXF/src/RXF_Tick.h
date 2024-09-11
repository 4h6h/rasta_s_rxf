/*
 * RXF_Tick 
 *
 * This file belongs to the Willert Software Tools RXF Product  
 * RXF_Rpy_C.
 *
 * Copyright (c) 2023, Willert Software Tools GmbH.
 *
 * E-mail: support(at)willert.de
 */


#ifndef RTOS_COORTX_RXF_Tick_H
#define RTOS_COORTX_RXF_Tick_H

#include "RXF_StandardTypes.h"
/*## package selectable::RTOS::COORTX */

/*## class TopLevel::RXF_Tick */
/*#[ ignore */
/** 
 *  RXF Class RXF_Tick
 */ 
/*#]*/
/***    User explicit entries    ***/


/* Operations */


/** 
 *  No tick init needed for COORTX because the tick source is provided from a third party.
 */ 
/*## operation RXF_Tick_Init() */
/* #MISRA_D4.9=OFF ID-0101: empty Function-like macro required because RXF_Tick is not used inside COORTX */   
#define RXF_Tick_Init()
/* #MISRA_D4.9=ON (end of ID-0101) */

#endif
