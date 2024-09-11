/*
 * RXF_TimerManager 
 *
 * This file belongs to the Willert Software Tools RXF Product  
 * RXF_Rpy_C.
 *
 * Copyright (c) 2023, Willert Software Tools GmbH.
 *
 * E-mail: support(at)willert.de
 */


#ifndef generic_RXF_TimerManager_H
#define generic_RXF_TimerManager_H

#include "RXF_StandardTypes.h"
#include "RXF_TimeoutEvent.h"
/*## package generic */

/*## class TopLevel::RXF_TimerManager */
/*#[ ignore */
/** 
 *  RXF Class RXF_TimerManager
 *
 *  The timer management class of the RXF which keeps track of the system ticks and the linked list of timeouts in use.
 */ 
/*#]*/

/** 
 *  Tick counter of the RXF, used as timing base for timeouts.
 */ 
/*## attribute rxfTicks */
#ifdef RXF_ANIMATION
extern RXF_TimeUnit_t rxfTicks;
#endif /* RXF_ANIMATION */ 


/***    User explicit entries    ***/


/* Operations */


/** 
 *  Schedules a timeout, managed by the RXF. So this function adds a timer to the queue of timeouts in use.
 *
 *    timeValueMs (In): Number of milliseconds before the timer should elapse. Not that there is a system tick granularity and the time  
 *  specified here is the minimum time after the timeout elapses.
 *  
 *    timeoutId (In): Id of the timeout inside the statechart
 *  
 *    destination (InOut): Pointer to the event object for the static timeout.
 *  
 */ 
/*#[ requirement(s) */
/**
 *  Satisfies requirement 4086: TimerManager - Track HighWaterMarks
 *  TimerManager shall keep track of the maximum number of TimeoutEvents used at the same time.
 *  
 *  Satisfies requirement 2855: TimerManager - Time Tick Translation
 *  TimerManager shall use a value in milliseconds to specify the delay of a TimeoutEvent.
 *  
 *  Satisfies requirement 2857: TimerManager - TimeoutEvent Initialization
 *  TimerManager shall initialize a TimeoutEvent with a given Reactive destination.
 *  
 *  Satisfies requirement 2851: TimerManager - Start timer
 *  TimerManager shall provide an operation to start the TimeoutEvent with a given delay time.
 *  
 */
/*#]*/
/*## operation start(RXF_TimeUnit_t,RXF_TimeoutEvent_ID_t,RXF_Reactive) */
void RXF_TimerManager_start(const RXF_TimeUnit_t timeValueMs, const RXF_TimeoutEvent_ID_t timeoutId, struct RXF_Reactive* const destination);


/** 
 *  Stops and cancels the given timeout using RXF timer management. This happens when e.g. a state is left before its  
 *  outgoing timer has elapsed.
 *
 *    timeoutID (In): Id of the timeout inside the statechart
 *  
 *    destination (In): Destination reactive object
 *  
 */ 
/*#[ requirement(s) */
/**
 *  Satisfies requirement 2896: TimerManager - Cancel Trigger
 *  TimerManager shall be able to identify if a canceled TimeoutEvent is canceled by itself or by another trigger.
 *  
 *  Satisfies requirement 2859: TimerManager - Cancel Queued Event
 *  If a TimeoutEvent is canceled while already queued, the TimerManager shall remove it from the EventQueue.
 *  
 *  Satisfies requirement 2852: TimerManager - Cancel
 *  TimerManager shall provide an operation to cancel a TimeoutEvent.
 *  
 */
/*#]*/
/*## operation stop(RXF_TimeoutEvent_ID_t,RXF_Reactive) */
void RXF_TimerManager_stop(const RXF_TimeoutEvent_ID_t timeoutID, const struct RXF_Reactive* const destination);


/** 
 *  Updates the processedTicks to the current rxfTicks value, calls the tick() operation and calls handleExpiredTimeouts().
 *  The casts to signed 16-bit int for the tick count are used to handle overflows of the RXF_TimerManager.rxfTicks correctly.
 *  The calculation is verified to be correct as long as the run-to-completion time does not exceed 32000 ticks.
 */ 
/*#[ requirement(s) */
/**
 *  Satisfies requirement 2853: TimerManager - Put in Queue
 *  If a TimeoutEvent is expired, the TimerManager shall put it into the associated EventQueue.
 *  
 *  Satisfies requirement 2854: TimerManager - Process Timeouts
 *  TimerManager shall provide an operation to process expired TimeoutEvents.
 *  
 */
/*#]*/
/*## operation processTimeouts() */
void RXF_TimerManager_processTimeouts(void);


/** 
 *  Initialize RXF timeout functionality.
 */ 
/*#[ requirement(s) */
/**
 *  Satisfies requirement 4037: TimerManager - TimoutEvent Pool
 *  TimerManager shall own a pool of TimeoutEvents
 *  
 *  Satisfies requirement 2860: TimerManager - Static Event Initialization
 *  TimerManager shall mark all TimeoutEvents as static at initialization.
 *  
 */
/*#]*/
/*## operation Init() */
void RXF_TimerManager_Init(void);


/** 
 *  Decrements a ticks from the remainingTicks attributes of the timeouts in use.
 */ 
/*#[ requirement(s) */
/**
 *  Satisfies requirement 2854: TimerManager - Process Timeouts
 *  TimerManager shall provide an operation to process expired TimeoutEvents.
 *  
 *  Satisfies requirement 2853: TimerManager - Put in Queue
 *  If a TimeoutEvent is expired, the TimerManager shall put it into the associated EventQueue.
 *  
 */
/*#]*/
/*## operation tick() */
void RXF_TimerManager_tick(void);


/** 
 *  Puts back a timeout to the list of unused timeouts.
 *
 *    timer (InOut): Timeout event to be put back.
 *  
 */ 
/*#[ requirement(s) */
/**
 *  Satisfies requirement 4086: TimerManager - Track HighWaterMarks
 *  TimerManager shall keep track of the maximum number of TimeoutEvents used at the same time.
 *  
 *  Satisfies requirement 2852: TimerManager - Cancel
 *  TimerManager shall provide an operation to cancel a TimeoutEvent.
 *  
 */
/*#]*/
/*## operation putBackTimer(RXF_Event) */
void RXF_TimerManager_putBackTimer(struct RXF_Event* const timer);


/** 
 *  Stops and cancels the given timeout using RXF timer management. This happens when e.g. a state is left before its  
 *  outgoing timer has elapsed.
 *
 *    destination (In): Destination reactive object
 *  
 */ 
/*#[ requirement(s) */
/**
 *  Satisfies requirement 4117: TimerManager - Cancel by Destination
 *  TimerManager shall be able to cancel all timeouts with a given Reactive destination.
 *  
 */
/*#]*/
/*## operation stopDestination(RXF_Reactive) */
void RXF_TimerManager_stopDestination(const struct RXF_Reactive* const destination);

/***    User explicit entries    ***/


#endif
