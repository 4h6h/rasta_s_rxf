/*
 * RXF_MainTask 
 *
 * This file belongs to the Willert Software Tools RXF Product  
 * RXF_Rpy_C.
 *
 * Copyright (c) 2023, Willert Software Tools GmbH.
 *
 * E-mail: support(at)willert.de
 */


#ifndef RTOS_COORTX_RXF_MainTask_H
#define RTOS_COORTX_RXF_MainTask_H

#include "RXF_StandardTypes.h"
#include "RXF_EventQueue.h"
#include "RXF_Startup.h"
/*## package selectable::RTOS::COORTX */

/*## class TopLevel::RXF_MainTask */
/*#[ ignore */
/** 
 *  RXF Class RXF_MainTask
 */ 
/*#]*/
/**  
 *  The OORTX only has one global message queue and no main task
 */
extern RXF_EventQueue mainMSQ;

/***    User explicit entries    ***/


/* Operations */


/** 
 *  Event based scheduler execution, it never ends. This function waits for messages from RXF_EventQueue for the task it is running  
 *  in to handle them by the Framework.
 */ 
/*#[ requirement(s) */
/**
 *  Satisfies requirement 5618: MainTask- Handle Events
 *  MainTask shall be able to handle Events in a FIFO manner.
 *  
 */
/*#]*/
/*## operation RXF_MainTask_executeAllEvents() */
void RXF_MainTask_executeAllEvents(void);


/** 
 *  Event based scheduler execution, it never ends. This function waits for messages from RXF_EventQueue for the task it is running  
 *  in to handle them by the Framework.
 */ 
/*#[ requirement(s) */
/**
 *  Satisfies requirement 5620: MainTask - Destination Check
 *  MainTask shall call the ErrorHandler, if a queued Event instance has no valid destination.
 *  
 *  Satisfies requirement 5619: MainTask - Event Forwarding
 *  Queued Events shall be forwarded to the associated Reactive destination.
 *  
 *  Satisfies requirement 5618: MainTask- Handle Events
 *  MainTask shall be able to handle Events in a FIFO manner.
 *  
 */

/* Forward declaration */
struct SandboxInstances;

/*#]*/
/*## operation RXF_MainTask_executeOneEvent() */
bool RXF_MainTask_executeOneEvent(void);


/** 
 */ 
/* #MISRA_D4.9=OFF ID-0007: Function-like macro required for Rhapsody compatibility. */   
// #define RXF_MainTask_start()    \
//     (void)modelMain();
/* #MISRA_D4.9=ON (end of ID-0007) */
int32_t RXF_MainTask_start(struct SandboxInstances* instances);
/***    User explicit entries    ***/


#endif
