/*
 * RXF_CriticalSection 
 *
 * This file belongs to the Willert Software Tools RXF Product  
 * RXF_Rpy_C.
 *
 * Copyright (c) 2023, Willert Software Tools GmbH.
 *
 * E-mail: support(at)willert.de
 */


#ifndef RTOS_COORTX_RXF_CriticalSection_H
#define RTOS_COORTX_RXF_CriticalSection_H

#include "RXF_StandardTypes.h"
/*## package selectable::RTOS::COORTX */

/*## class TopLevel::RXF_CriticalSection */
/*#[ ignore */
/** 
 *  RXF Class RXF_CriticalSection
 */ 
/*#]*/

/** 
 *  No critical sections needed for single threaded COORTX.
 */ 
#define RXF_CRITICALSECTION_VARIABLE

/** 
 *  No critical sections needed for single threaded COORTX.
 */ 
#define RXF_CRITICALSECTION_LOCK_CALL

/** 
 *  No critical sections needed for single threaded COORTX.
 */ 
#define RXF_CRITICALSECTION_UNLOCK_CALL

#endif
