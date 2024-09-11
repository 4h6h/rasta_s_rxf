/*
 * RXF_Event 
 *
 * This file belongs to the Willert Software Tools RXF Product  
 * RXF_Rpy_C.
 *
 * Copyright (c) 2023, Willert Software Tools GmbH.
 *
 * E-mail: support(at)willert.de
 */


#ifndef generic_RXF_Event_H
#define generic_RXF_Event_H

#include "RXF_StandardTypes.h"
#include "RXF_Monitor.h"
struct RXF_Reactive;

/*## package generic */

/*## class RXF_Event */
typedef struct RXF_Event RXF_Event;

/** 
 *  Type to identify an event. The IDs are generated from Rhapsody.
 *  The name is required to satisfy the needs of the MISRA2004 Settings in Rhapsody.
 */ 
typedef int16_t RXFEventId;


/** 
 *  Define to support Rhapsody code generator naming of a timeout ID.
 */ 
#define Timeout_id RXF_Event_Timeout_id

/** 
 *  Function-like-macro to support Rhapsody code generator naming to retrieve a timeout ID.
 */ 
/* #MISRA_D4.9=OFF ID-0007: Function-like macro required for Rhapsody compatibility. */   
#define RiCTimeout_getTimeoutId(EVENT) ((RXF_TimeoutEvent*)(EVENT))->timer_id
/* #MISRA_D4.9=ON (end of ID-0007) */

/** 
 *  The identifier of all timeout events.
 */ 
typedef RXF_Event RiCTimeout;

/** 
 *  Null event id, used to trigger null-transisions (which do not have a trigger specified).
 */ 
#define Null_id RXF_Event_Null_id

/** 
 *  event id for null events
 */ 
#define RXF_Event_Null_id -1

/** 
 *  event id for timeout events
 */ 
#define RXF_Event_Timeout_id -2
/** 
 *  RXF Class RXF_Event
 *
 *  This class represents a single event.
 *  
 *  Events are asynchronous messages which are handled by the RXF event based scheduler. 
 *  Usually events are dynamically allocated and freed (via static memory pools), but also static events which do not get destroyed are supported. 
 *  They are used for example when events are being generated from an interrupt service routine.
 */ 
struct RXF_Event
{
    
    /** 
     *  Pointer to the next event in a linked list. This is used to manage events in the event queue and timeouts in the list of  
     *  active timeouts (waiting to elapse).
     */ 
    struct RXF_Event* next;		/*## attribute next */
    
    /** 
     *  The destination FSM that shall consume the event.
     */ 
    struct RXF_Reactive* destination;		/*## attribute destination */
    
    /** 
     *  Identifier of the event. Rhapsody code generation required the name to exactly be "lId".
     */ 
    RXFEventId lId;		/*## attribute lId */
    
    /** 
     *  Additional attributes for LiveAnimation
     */ 
    RXF_MONITOR_ADDITIONAL_EVENT_PARAMETER		/*## attribute RXF_MONITOR_ADDITIONAL_EVENT_PARAMETER */
    
    /** 
     *  Flag to indicate if the event object is allocated static or dynamic.
     */ 
    bool isStatic;		/*## attribute isStatic */
};

/***    User implicit entries    ***/


/* Constructors and destructors:*/


/** 
 *  Initializes the event object.
 *  The name property is overridden, as Rhapsody expects the initializer to begin with an lower case "i".
 *
 *    eventId (In): ID to be assigned to the event object.
 *  
 *    destinationFSM (InOut): Pointer to the reactive object which should receive the event.
 *  
 */ 
/*#[ requirement(s) */
/**
 *  Satisfies requirement 2843: Event - Identifier
 *  An Event instance shall be able to be identified by an id.
 *  
 *  Satisfies requirement 17: Event - Reactive Destination
 *  Every Event instance shall store a Reactive destination.
 *  
 */
/*#]*/
/*## operation Init(RXFEventId,RXF_Reactive) */
void RXF_Event_init(RXF_Event* const me, const RXFEventId eventId, struct RXF_Reactive* const destinationFSM);

/***    User explicit entries    ***/


/* Operations */


/** 
 *  Marks an event as dynamic allocated or static.
 *  The information is needed by the RXF if the event memory needs to returned.
 *
 *    value (In): true == event is static / false == event is dynamic allocated
 *  
 */ 
/**
 *  Satisfies requirement 2845: Event - Default Deletion
 *  Event shall provide a mechanism to identify whether an Event instance is statically or dynamically allocated.
 *  
 *//*## operation setDeleteAfterConsume(uint32_t) */
/* #MISRA_D4.9=OFF ID-0007: Function-like macro required for Rhapsody compatibility. */   
#define RXF_Event_setDeleteAfterConsume(me, value)    \
    (me)->isStatic = !(value);
/* #MISRA_D4.9=ON (end of ID-0007) */

#endif
