/*
 * RXF_TimeoutEvent 
 *
 * This file belongs to the Willert Software Tools RXF Product  
 * RXF_Rpy_C.
 *
 * Copyright (c) 2023, Willert Software Tools GmbH.
 *
 * E-mail: support(at)willert.de
 */


#ifndef generic_RXF_TimeoutEvent_H
#define generic_RXF_TimeoutEvent_H

#include "RXF_StandardTypes.h"
#include "RXF_Event.h"
/*## package generic */

/*## class RXF_TimeoutEvent */
typedef struct RXF_TimeoutEvent RXF_TimeoutEvent;

/** 
 *  For timeout IDs the same type as for event IDs is used.
 */ 
typedef RXFEventId RXF_TimeoutEvent_ID_t;


/** 
 *  The basic type used as time unit.
 */ 
typedef uint32_t RXF_TimeUnit_t;

/** 
 *  RXF Class RXF_TimeoutEvent
 *
 *  Class representing timeout events.
 *  They consist of RXF_Event plus timeout specific attributes.
 */ 
struct RXF_TimeoutEvent
{
    
    /** 
     *  A timeout consists of an event plus additional attributes.
     */ 
    struct RXF_Event event;		/*## attribute event */
    
    /** 
     *  Pointer to the previous event inside the busy list.
     */ 
    struct RXF_TimeoutEvent* previous;		/*## attribute previous */
    
    /** 
     *  Identifier specific to a timeout. Not to be confused with the event id, which just identifies this event to be a timeout  
     *  event.
     */ 
    RXF_TimeoutEvent_ID_t timer_id;		/*## attribute timer_id */
    
    /** 
     *  Number of system ticks that remain, before the timeout has elapsed.
     */ 
    RXF_TimeUnit_t remainingTicks;		/*## attribute remainingTicks */
};

#endif
