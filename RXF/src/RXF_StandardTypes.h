/*
 * RXF_StandardTypes 
 *
 * This file belongs to the Willert Software Tools RXF Product  
 * RXF_Rpy_C.
 *
 * Copyright (c) 2023, Willert Software Tools GmbH.
 *
 * E-mail: support(at)willert.de
 */


#ifndef RTOS_COORTX_RXF_StandardTypes_H
#define RTOS_COORTX_RXF_StandardTypes_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/*## package selectable::RTOS::COORTX */

/*## class TopLevel::RXF_StandardTypes */
/*#[ ignore */
/** 
 *  RXF Class RXF_StandardTypes
 */ 
/*#]*/

/** 
 *  Mapping int to int_t
 */ 
typedef int int_t;


/* Typedefs */
typedef uint32_t MsgId_t;
typedef uint32_t MsgLen_t;
typedef uint32_t NodeId_t;
typedef uint32_t SpduLen_t;
typedef uint32_t ConnId_t;

typedef enum {
    OK = 0U,
    NOT_OK
} StdRet_t;

#define UNUSED(x) (void)(x)

#endif
