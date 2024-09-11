/*
 * RXF_Reactive 
 *
 * This file belongs to the Willert Software Tools RXF Product  
 * RXF_Rpy_C.
 *
 * Copyright (c) 2023, Willert Software Tools GmbH.
 *
 * E-mail: support(at)willert.de
 */


#ifndef RTOS_COORTX_RXF_Reactive_H
#define RTOS_COORTX_RXF_Reactive_H

#include "RXF_StandardTypes.h"
#include "RXF_Event.h"
#include "RXF_TimeoutEvent.h"
/*## package selectable::RTOS::COORTX */

/*## class RXF_Reactive */
typedef struct RXF_Reactive RXF_Reactive;

/** 
 *  Possible Rhapsody event consumption return status.
 */ 
typedef enum RXFTakeEventStatus
{
    eventNotConsumed = 0, /* The event could not be consumed, e.g. because the receiving object is not in a state which handles that event. */
    eventConsumed = 1 /* Handling of the event was performed succesfull. */
} RXFTakeEventStatus;


/** 
 *  Initial status of a reactive object.
 */ 
#define RXF_Reactive_statusDefault ((uint8_t) 0x00U)

/** 
 *  Status of an FSM: FSM is busy.
 */ 
#define RXF_Reactive_statusBusy ((uint8_t) 0x01U)

/** 
 *  Status of an FSM: FSM is busy.
 */ 
#define RXF_Reactive_statusBusyInverted ((uint8_t) 0xFEU)

/** 
 *  Status of an FSM: FSM is started.
 */ 
#define RXF_Reactive_statusStarted ((uint8_t) 0x02U)

/** 
 *  Status of an FSM: Flag to toggle if information are send to UML Target Debugger
 */ 
/* #MISRA_2.5=OFF ID-0006: Required for RXF Monitor compatibility. Unused when Monitor is deactivated. */   
#define RXF_Reactive_statusFiltered ((uint8_t) 0x04U)
/* #MISRA_2.5=ON (end of ID-0006) */

/** 
 *  Status of an FSM: FALSE. Used to bit-wise compare status flags with the non-set status.
 */ 
#define RXF_Reactive_statusFALSE (RXF_Reactive_statusDefault)

/** 
 *  For Rhapsody framework compatibility: Mapping macro to <state>_isCompleted.
 */ 
/* #MISRA_D4.9,20.10=OFF ID-0008: Function-like macro using concatenation operator required for Rhapsody compatibility. */   
#define IS_COMPLETED(me,state) (state##_isCompleted(me))
/* #MISRA_D4.9,20.10=ON (end of ID-0008) */

/** 
 *  For Rhapsody framework compatibility: Mapping macro to <state>_IN
 */ 
/* #MISRA_D4.9,20.10=OFF ID-0008: Function-like macro using concatenation operator required for Rhapsody compatibility. */   
#define IS_IN(me,state) (state##_IN(me))
/* #MISRA_D4.9,20.10=ON (end of ID-0008) */

/** 
 *  For Rhapsody framework compatibility. Used to set the optional return value for triggered operation calls.
 */ 
/* #MISRA_D4.9=OFF ID-0007: Function-like macro required for Rhapsody compatibility. */   
#define CREPLY(retVal) (params->ric_reply = (retVal))
/* #MISRA_D4.9=ON (end of ID-0007) */
/** 
 *  RXF Class RXF_Reactive
 *
 *  Class for FSM (finite state machine) behavior aggregated by all classes that use a statechart.
 */ 
struct RXF_Reactive
{
    
    /** 
     *  Virtual table of functions to process events is only used by Rhapsody for compatibility reasons.
     */ 
    const struct RXF_Reactive_Vtbl * vtbl;
    
    /** 
     *  current event processed by the state machine
     */ 
    struct RXF_Event* current_event;
    
    /** 
     *  Status of the reactive object, see RXF_Reactive_status... for Details.
     */ 
    uint8_t status;
    
    /** 
     *  Used to count the number of null transitions (transitions that do not have a trigger).
     */ 
    uint8_t nullTransitionCount;
};

/***    User explicit entries    ***/


/* Operations */


/** 
 *  Generate an event for this statemachine.
 *  Returns true, if the event could be added to the message queue.
 *
 *    event (InOut): Event pointer for the event to be put in the message queue.
 *  
 */ 
/**
 *  Satisfies requirement 4270: Reactive - Event Reuse Protection
 *  A Reactive instance shall provide a reuse protection for Events to ensure an Event exist only once in an EventQueue at the same time.
 *  
 *  Satisfies requirement 2880: Reactive - Receive Event
 *  Reactive shall provide an operation to receive an Event, that should be queued in the associated EventQueue.
 *  
 */
/*## operation gen(RXF_Event) */
void RXF_Reactive_gen(RXF_Reactive* const me, struct RXF_Event* const event RXF_MONITOR_FSM_GEN_SOURCE_ARGUMENT);


/** 
 *  Starts the behavior of the state machine. Returns true, if the the behavior had not been not started before and was  
 *  successfully started.
 */ 
/**
 *  Satisfies requirement 3280: Reactive - Initial Run To Completion
 *  Existing null transitions outgoing from the initial state shall be processed by the creating context.
 *  
 *  Satisfies requirement 2890: Reactive - Run To Completion
 *  Reactive shall fulfill the run to completion behavior of a statechart.
 *  
 *  Satisfies requirement 2892: Reactive - Restart Protection
 *  A Reactive instance shall only start its behavior once. Every following start attempt shall call the ErrorHandler.
 *  
 *  Satisfies requirement 3278: Reactive - Triggered Operations Start Be...
 *  Reactive shall only start processing triggered operations after startBehavior() is called.
 *  
 *  Satisfies requirement 2881: Reactive - Events Start Behavior
 *  Reactive shall only start processing Events after startBehavior() is called.
 *  
 */
/*## operation startBehavior() */
bool RXF_Reactive_startBehavior(RXF_Reactive* const me);


/** 
 *  Initialize state machine (data only, not starting the state machine)
 *
 *    vtbl (In): Pointer to the virtual table containing function pointers to Rhapsody generated functions.
 *  
 */   
/**
 *  Satisfies requirement 4090: Reactive - Initialization
 *  It shall be possible to initialize a Reactive instance with a given virtual table.
 *  
 *  Satisfies requirement 2892: Reactive - Restart Protection
 *  A Reactive instance shall only start its behavior once. Every following start attempt shall call the ErrorHandler.
 *  
 */
/*## operation init(RXF_Reactive_Vtbl) */
void RXF_Reactive_init(RXF_Reactive* const me, const struct RXF_Reactive_Vtbl* vtbl);


/** 
 *  Handles the status of the statemachine (including NULL transition handling / run to completion)
 *  and calls the receiving object's dispatch event operation.
 *
 *    event (InOut): Event to be consumed.
 *  
 */ 
/**
 *  Satisfies requirement 2890: Reactive - Run To Completion
 *  Reactive shall fulfill the run to completion behavior of a statechart.
 *  
 */
/*## operation consumeGivenEvent(RXF_Event) */
void RXF_Reactive_consumeGivenEvent(RXF_Reactive* const me, struct RXF_Event* const event);


/** 
 *  Removes all events from the depending EventQueue and stops all depending timeouts.
 */ 
/**
 *  Satisfies requirement 3279: Reactive - Triggered Operation Stop Proc...
 *  Reactive shall stop processing triggered operations when cleanup() is called.
 *  
 *  Satisfies requirement 2882: Reactive - Event Stop Processing
 *  Reactive shall stop processing Events when cleanup() is called.
 *  
 */
/*## operation cleanup() */
void RXF_Reactive_cleanup(const RXF_Reactive* const me);


/** 
 *  Remove an timeout event from the message queue
 *
 *    timerID (In): id of the timer inside the statechart
 *  
 */ 
/**
 *  Satisfies requirement 4034: Reactive - Cancel Timer
 *  It shall be possible to cancel a UML Timeout.
 *  
 */
/*## operation cancelTimeout(RXF_TimeoutEvent_ID_t) const */
void RXF_Reactive_cancelTimeout(const RXF_Reactive* const me, const RXF_TimeoutEvent_ID_t timerID);


/**
 *  Sets if the object is in started status.
 */ 
/**
 *  Satisfies requirement 2893: Reactive - Deletion
 *  A dynamically allocated Reactive instance shall be deletable.
 *  
 */
void RXF_Reactive_terminate(const RXF_Reactive* const me);


/**
 *  This macro is not used by the RXF but Rhapsody generates it.
 */ 
/* #MISRA_D4.9=OFF ID-0007: Function-like macro required for Rhapsody compatibility. */   
#define DYNAMICALLY_ALLOCATED(me)
/* #MISRA_D4.9=ON (end of ID-0007) */


/**
 *  Decrement null-transition counter.
 */ 
/**
 *  Satisfies requirement 2891: Reactive - Infinite Null Transitions
 *  Reactive shall detect cyclic null transitions and call the ErrorHandler in such a case.
 *  
 */
void RXF_Reactive_popNullConfig(RXF_Reactive* const me);


/**
 *  Increment null-transition counter.
 */ 
/**
 *  Satisfies requirement 2891: Reactive - Infinite Null Transitions
 *  Reactive shall detect cyclic null transitions and call the ErrorHandler in such a case.
 *  
 */
void RXF_Reactive_pushNullConfig(RXF_Reactive* const me);


/**
 *  Handling of an event. After checking the validity of the event and the statemachine status, the event is passed via  
 *  virtual table to the generated code of a class with statemachine. Also handling of NULL events and event which have not  
 *  been consumed by the generated statemachine code is handled in this function and its private sub-functions.
 *
 *    event (InOut): Pointer to the event object to be handled.
 *  
 */ 
/**
 *  Satisfies requirement 2887: Reactive - Handle Event
 *  Reactive shall provide an operation to process an Event, that returns from the EventQueue.
 *  
 *  Satisfies requirement 3279: Reactive - Triggered Operation Stop Proc...
 *  Reactive shall stop processing triggered operations when cleanup() is called.
 *  
 *  Satisfies requirement 2882: Reactive - Event Stop Processing
 *  Reactive shall stop processing Events when cleanup() is called.
 *  
 */
void RXF_Reactive_takeEvent(RXF_Reactive* const me, struct RXF_Event* const event);


/**
 *  Called from synchronous events, also known as triggered operations. Mapped to handle it just like a normal event from  
 *  the framework side.
 *
 *    event (InOut): Event related to the triggered operation call
 *  
 */ 
/**
 *  Satisfies requirement 2894: Reactive - Cyclic Triggered Operations
 *  Cyclic triggered operation calls shall be detected by Reactive and the ErrorHandler shall be called.
 *  
 *  Satisfies requirement 2888: Reactive - Handle Triggered Operation
 *  Reactive shall provide an operation to process a triggered operation.
 *  
 */
void RXF_Reactive_takeTrigger(RXF_Reactive* const me, struct RXF_Event* const event);

#endif
