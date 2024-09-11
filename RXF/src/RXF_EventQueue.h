/*
 * RXF_EventQueue 
 *
 * This file belongs to the Willert Software Tools RXF Product  
 * RXF_Rpy_C.
 *
 * Copyright (c) 2023, Willert Software Tools GmbH.
 *
 * E-mail: support(at)willert.de
 */


#ifndef RTOS_COORTX_RXF_EventQueue_H
#define RTOS_COORTX_RXF_EventQueue_H

#include "RXF_StandardTypes.h"
#include "RXF_TimeoutEvent.h"
/*## package selectable::RTOS::COORTX */

/*## class RXF_EventQueue */
/** 
 *  RXF Class RXF_EventQueue
 *
 *  Message queue functionality in RXF.
 */ 
typedef struct 
{
    
    /** 
     *  Pointer to the head of the FIFO event queue.
     *  NULL, if there are currently no events in the queue. 
     */ 
    struct RXF_Event* head;
    
    /** 
     *  Pointer to the tail of the FIFO event queue.
     *  NULL, if there are currently no events in the queue.
     */ 
    struct RXF_Event* tail;
} RXF_EventQueue;

/***    User implicit entries    ***/


/* Constructors and destructors:*/


/**
 *  Initializes the message queue by setting its head and tail pointer to NULL.
 */ 
/**
 *  Satisfies requirement 5617: EventQueue - Init
 *  An EventQueue shall provide an operation to initialize it.
 *  
 *  Satisfies requirement 193: EventQueue - FIFO Behaviour
 *  An EventQueue shall be able to handle events in a FIFO manner.
 *  
 */
void RXF_EventQueue_Init(RXF_EventQueue* const me);

/***    User explicit entries    ***/


/* Operations */


/**
 *  Removes all events with a specific destination from a message queue
 *
 *    destination (In): Destination for an event which needs to be canceled
 *  
 */ 
/**
 *  Satisfies requirement 163: EventQueue - Cancel Event via Destination
 *  It shall be possible to cancel queued events via their associated reactive destination.
 *  
 */
void RXF_EventQueue_cancelEvents(RXF_EventQueue* const me, const struct RXF_Reactive* const destination);


/**
 *  Remove a timeout event from the message queue
 *
 *    destination (In): Destination for a timeout event which needs to be canceled
 *  
 *    timerID (In): id of the timer inside the statechart
 *  
 */ 
/**
 *  Satisfies requirement 49: EventQueue- Remove Queued Timeout
 *  A queued timeout event shall be removable.
 *  
 */
void RXF_EventQueue_cancelTimeout(RXF_EventQueue* const me, const struct RXF_Reactive* const destination, const RXF_TimeoutEvent_ID_t timerID);


/**
 *  Retrieve an event from the message queue's head position.
 *  When the message queue is empty, a NULL pointer is returned.
 */ 
/**
 *  Satisfies requirement 4092: EventQueue - Empty Returnvalue
 *  When an EventQueue is empty and an Event is requested from the queue, it shall return NULL.
 *  
 *  Satisfies requirement 193: EventQueue - FIFO Behaviour
 *  An EventQueue shall be able to handle events in a FIFO manner.
 *  
 */
struct RXF_Event* RXF_EventQueue_get(RXF_EventQueue* const me);


/** 
 */ 
/**
 *  Satisfies requirement 38: EventQueue - Empty Check
 *  It shall be possible to check if an EventQueue is empty.
 *  
 */
bool RXF_EventQueue_isEmpty(const RXF_EventQueue* const me);


/**
 *  Puts an event into the message queue at the tail position.
 *
 *    event (InOut): Event to be queued
 *  
 */ 
/**
 *  Satisfies requirement 193: EventQueue - FIFO Behaviour
 *  An EventQueue shall be able to handle events in a FIFO manner.
 *  
 */
void RXF_EventQueue_put(RXF_EventQueue* const me, struct RXF_Event* const event);

#endif
