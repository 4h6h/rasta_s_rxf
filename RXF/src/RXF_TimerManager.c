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

#include "log.h"

#include "RXF_TimerManager.h"
#include "RXF_Config.h"
#include "RXF_CriticalSection.h"
#include "RXF_ErrorHandler.h"
#include "RXF_HighWaterMarks.h"
#include "RXF_Reactive.h"
/*## package generic */

/*## class TopLevel::RXF_TimerManager */

/** 
 *  Pointer to the first timeout in use.
 *  The first timeout is also the one with the smallest remaining ticks value.
 */ 
/*## attribute timeoutsInUse */
static struct RXF_TimeoutEvent* timeoutsInUse = NULL;


/** 
 *  Pointer to the first free timeout.
 */ 
/*## attribute timeoutsFree */
static struct RXF_TimeoutEvent* timeoutsFree;

/*## attribute rxfTicks */
#ifdef RXF_ANIMATION
RXF_TimeUnit_t rxfTicks = 0U;
#endif /* RXF_ANIMATION */ 



/** 
 *  Converts the given timeout value from milliseconds to number of ticks.
 *
 *    delayTime (In): Timeout value in milliseconds
 *  
 */ 
/*#[ requirements:
 *  Satisfies requirement 2855: TimerManager - Time Tick Translation
 *  TimerManager shall use a value in milliseconds to specify the delay of a TimeoutEvent.
 *   */
/*#]*/
/*## operation timeToTicks(RXF_TimeUnit_t) */
static inline RXF_TimeUnit_t timeToTicks(const RXF_TimeUnit_t delayTime);

/*## operation start(RXF_TimeUnit_t,RXF_TimeoutEvent_ID_t,RXF_Reactive) */
void RXF_TimerManager_start(const RXF_TimeUnit_t timeValueMs, const RXF_TimeoutEvent_ID_t timeoutId, struct RXF_Reactive* const destination)
{
    RXF_TimeoutEvent* timeoutEvent;
    RXF_TimeUnit_t ticks;
    RXF_CRITICALSECTION_VARIABLE
    		
	LOG_INFO ("RXF_TimerManager_start %u", timeoutId);

    ticks = timeToTicks( timeValueMs );
    
    RXF_CRITICALSECTION_LOCK_CALL
    
    if( timeoutsFree != NULL )
    {
    	
    	#ifdef RXF_HIGH_WATER_MARKS_ENABLED
    	RXF_HighWaterMarks.timerInUseCount++;
    	if( RXF_HighWaterMarks.timerInUseCount > RXF_HighWaterMarks.timerInUseMax )
    	{
    		RXF_HighWaterMarks.timerInUseMax++;
    	}
    	#endif /* RXF_HIGH_WATER_MARKS_ENABLED */
    
    	timeoutEvent = timeoutsFree;
    	/* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_Event to RXF_TimeoutEvent */
    	timeoutsFree = (RXF_TimeoutEvent*)timeoutsFree->event.next;
    	/* #MISRA_11.3=ON */
    	
    	RXF_CRITICALSECTION_UNLOCK_CALL
    
    	timeoutEvent->timer_id = timeoutId;
    	timeoutEvent->event.destination = destination;
    	
    	/* if a tm( 0 ) is used directly put timeoutevent into the message queue */
    	if( 0U == timeValueMs )
    	{
    		RXF_Reactive_gen( destination, &timeoutEvent->event RXF_MONITOR_TMM_GEN_SOURCE_ARGUMENT);
    	}
    	else
    	{	
    		timeoutEvent->remainingTicks = ticks;
    							
    		RXF_CRITICALSECTION_LOCK_CALL
    		/* And insert timeout into the busy list */
    		/* List empty First element */
    		if ( timeoutsInUse == NULL )
    		{
    			/* The list was empty so we place it in the list.
    	
    					 Before: ->NULL
    					 New:    [150]
    					 After:  [150]->NULL
    			 */
    			
    			timeoutsInUse = timeoutEvent;
    			timeoutEvent->event.next = NULL;
    			timeoutEvent->previous = NULL;	
    		}
    		else
    		{
    			/* Check if the new element must be placed _before_ the first in the busy list */
    			if ( timeoutsInUse->remainingTicks >= timeoutEvent->remainingTicks )
    			{    				
    				/* Yes must be before. Do it now. */
    				timeoutEvent->event.next = &timeoutsInUse->event;
    				timeoutsInUse->previous = timeoutEvent;
    				timeoutsInUse = timeoutEvent;
    				
    				/* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_Event to RXF_TimeoutEvent */
    				((RXF_TimeoutEvent*)timeoutEvent->event.next)->remainingTicks -= timeoutEvent->remainingTicks;
    				/* #MISRA_11.3=ON */
    				
    				/* The next elements time must be decreased with the new elements
    					 time. In that way we store the time relative.
    	
    						 Before: [100]->[100]->[25]->NULL
    						 New:    [50]
    						 After:  [50]->[50]->[100]->[25]->NULL
    				 */
    			}
    			else
    			{
    				RXF_TimeoutEvent *tempTmrPtr;
    				/* No it must be after the first element. Good, now we only have to
    					 walk acros the list. Now let tempTmrPtr point to the first element
    				 */
    				
    				tempTmrPtr = timeoutsInUse;
    	
    				/* Since we store the time relative we must subtract the first element's
    					 time from the new elements time.
    				 */
    				timeoutEvent->remainingTicks -= tempTmrPtr->remainingTicks;
    	
    				/* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_Event to RXF_TimeoutEvent */
    				while( (tempTmrPtr->event.next != NULL) &&
    				 
    						 (timeoutEvent->remainingTicks > ((RXF_TimeoutEvent*)tempTmrPtr->event.next)->remainingTicks) )
    				{
    				
    					timeoutEvent->remainingTicks -= ((RXF_TimeoutEvent*)tempTmrPtr->event.next)->remainingTicks;
    					tempTmrPtr = (RXF_TimeoutEvent*)tempTmrPtr->event.next;
    				}
    				/* #MISRA_11.3=ON */
    				
    				/* OK, we found the element where we place the new one after
    					 This can be the last, the last element's next pointer is NULL
    					 but we can handle that.
    				 */
    				timeoutEvent->event.next = tempTmrPtr->event.next;
    				timeoutEvent->previous = tempTmrPtr;
    				/* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_TimeoutEvent to RXF_Event */
    				tempTmrPtr->event.next = (RXF_Event*)timeoutEvent;
    				/* #MISRA_11.3=ON */
    				if( timeoutEvent->event.next != NULL )
    				{
    					/* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_Event to RXF_TimeoutEvent */
    					((RXF_TimeoutEvent*)timeoutEvent->event.next)->remainingTicks -= timeoutEvent->remainingTicks;
    					((RXF_TimeoutEvent*)timeoutEvent->event.next)->previous = timeoutEvent;
    					/* #MISRA_11.3=ON */
    				}
    			} /* else: new timeout will not be first which expires. */
    		}	/* else: timeout busy list is not empty. */
    		RXF_CRITICALSECTION_UNLOCK_CALL				
    	} /* else: timout is no tm( 0 ) */			
    }
    else
    {
    	RXF_CRITICALSECTION_UNLOCK_CALL
    	RXF_ErrorHandler_error( RXF_ERROR_TIMER_START_FAILED, 0, true );
    }
}

/*## operation stop(RXF_TimeoutEvent_ID_t,RXF_Reactive) */
void RXF_TimerManager_stop(const RXF_TimeoutEvent_ID_t timeoutID, const struct RXF_Reactive* const destination)
{
    /* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_Event to RXF_TimeoutEvent */
    if( ( (RXFEventId)RXF_Event_Timeout_id != destination->current_event->lId ) ||
    	  ( timeoutID != ((RXF_TimeoutEvent*)destination->current_event)->timer_id ) ) /* Check if we cancel timeout which is not currently consumed */
    /* #MISRA_11.3=ON */
    {
    	RXF_CRITICALSECTION_VARIABLE
    	RXF_TimeoutEvent *tempTmrPtr;
    	RXF_CRITICALSECTION_LOCK_CALL
    	tempTmrPtr = timeoutsInUse; 
    	while (tempTmrPtr != NULL)
    	{
    		if ( ( tempTmrPtr->event.destination == destination ) &&
    			 ( tempTmrPtr->timer_id == timeoutID ) )
    		{
    			if( tempTmrPtr == timeoutsInUse )
    			{
    				/* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_Event to RXF_TimeoutEvent */
    				timeoutsInUse = ( RXF_TimeoutEvent* )tempTmrPtr->event.next;
    				/* #MISRA_11.3=ON */
    			}
    			else 
    			{
    				tempTmrPtr->previous->event.next = tempTmrPtr->event.next;
    			}
    			
    			if( NULL != tempTmrPtr->event.next )
    			{
    				/* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_Event to RXF_TimeoutEvent */
    				((RXF_TimeoutEvent*)tempTmrPtr->event.next)->remainingTicks += tempTmrPtr->remainingTicks;
    				((RXF_TimeoutEvent*)tempTmrPtr->event.next)->previous = tempTmrPtr->previous;
    				/* #MISRA_11.3=ON */
    			}
    			RXF_TimerManager_putBackTimer(&tempTmrPtr->event);
    			break;
    		}
    		/* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_Event to RXF_TimeoutEvent */
    		tempTmrPtr = (RXF_TimeoutEvent*)tempTmrPtr->event.next;
    		/* #MISRA_11.3=ON */
    	}
    	RXF_CRITICALSECTION_UNLOCK_CALL
    
    	/* if timeout is already inside the message queue as UML event, remove it from message queue. */
    	if (tempTmrPtr == NULL)
    	{
    		RXF_Reactive_cancelTimeout( destination, timeoutID );
    	}
    }
}

/*## operation processTimeouts() */
void RXF_TimerManager_processTimeouts(void)
{
    RXF_TimeoutEvent* timeoutEvent = NULL;

	LOG_INFO ("RXF_TimerManager_processTimeouts %p", timeoutsInUse);
    
    while ( ( timeoutsInUse != NULL ) && 
    		( timeoutsInUse->remainingTicks == (RXF_TimeUnit_t) 0 ) )
    {
    	RXF_CRITICALSECTION_VARIABLE
    	RXF_CRITICALSECTION_LOCK_CALL
    		
    	timeoutEvent = timeoutsInUse;
    	
    	/* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_Event to RXF_TimeoutEvent */
    	timeoutsInUse = (RXF_TimeoutEvent*)timeoutEvent->event.next;
    	/* #MISRA_11.3=ON */
    	
    	RXF_CRITICALSECTION_UNLOCK_CALL
    	
    	RXF_Reactive_gen(timeoutEvent->event.destination, &timeoutEvent->event RXF_MONITOR_TMM_GEN_SOURCE_ARGUMENT);
    }
}

static inline RXF_TimeUnit_t timeToTicks(const RXF_TimeUnit_t delayTime)
{
    RXF_TimeUnit_t ticks;
    
    ticks = (RXF_TimeUnit_t) ( ( (delayTime + RXF_MS_PER_TICK) - 1U ) / RXF_MS_PER_TICK );
    
    #ifdef RXF_TIMEOUT_GUARANTEE_TIME
    /** The tick value needs to be incremented by one, as the OORTX timer management would expire
     *  a tm(1 tick) at the first occurrence of a tick (which might be too short).
     */
    ticks++;
    #endif
    
    return ticks;
}

/*## operation Init() */
void RXF_TimerManager_Init(void)
{
	LOG_INFO("RXF_TimerManager_Init");
    /* Memory for all timer objects */ 
    static struct RXF_TimeoutEvent timerPool[RXF_MAX_TIMEOUTS];
    
    uint32_t i;
    for( i = 0U; i < ( RXF_MAX_TIMEOUTS - 1U ) ; i++)
    {
    	/* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_TimeoutEvent to RXF_Event */
    	timerPool[ i ].event.next = (RXF_Event*)&timerPool[ i + 1U ];
    	/* #MISRA_11.3=ON */
    	timerPool[ i ].event.isStatic = true;
    	timerPool[ i ].event.lId = (RXFEventId)RXF_Event_Timeout_id;
    }
    timerPool[ RXF_MAX_TIMEOUTS - 1U ].event.next = NULL;
    timerPool[ RXF_MAX_TIMEOUTS - 1U ].event.isStatic = true;
    timerPool[ RXF_MAX_TIMEOUTS - 1U ].event.lId = (RXFEventId)RXF_Event_Timeout_id;
    timeoutsFree = &timerPool[ 0U ];
    
    #ifdef RXF_ANIMATION
    rxfTicks = 0U;
    #endif /* RXF_ANIMATION */
}

/*## operation tick() */
void RXF_TimerManager_tick(void)
{
    #ifdef RXF_ANIMATION
    rxfTicks++;
    #endif /* RXF_ANIMATION */


    
    if( timeoutsInUse != NULL )
    {
    	timeoutsInUse->remainingTicks--;
    }
}

/*## operation putBackTimer(RXF_Event) */
void RXF_TimerManager_putBackTimer(struct RXF_Event* const timer)
{
    RXF_CRITICALSECTION_VARIABLE						
    RXF_CRITICALSECTION_LOCK_CALL
    
    /* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_Event to RXF_TimeoutEvent and vice versa */
    timer->next = (RXF_Event*)timeoutsFree;
    timeoutsFree = (RXF_TimeoutEvent*)timer;
    /* #MISRA_11.3=ON */
    
    #ifdef RXF_HIGH_WATER_MARKS_ENABLED
    RXF_HighWaterMarks.timerInUseCount--;
    #endif /* RXF_HIGH_WATER_MARKS_ENABLED */
    
    RXF_CRITICALSECTION_UNLOCK_CALL
}

/*## operation stopDestination(RXF_Reactive) */
void RXF_TimerManager_stopDestination(const struct RXF_Reactive* const destination)
{
    RXF_TimeoutEvent *tempTmrPtr;
    RXF_TimeoutEvent *next;
    RXF_CRITICALSECTION_VARIABLE
    RXF_CRITICALSECTION_LOCK_CALL
    tempTmrPtr = timeoutsInUse;
    while (tempTmrPtr != NULL)
    {	
    	/* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_Event to RXF_TimeoutEvent */
    	next = (RXF_TimeoutEvent*)tempTmrPtr->event.next;
    	/* #MISRA_11.3=ON */
    	if ( tempTmrPtr->event.destination == destination )
    	{
    		if( tempTmrPtr == timeoutsInUse )
    		{
    			/* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_Event to RXF_TimeoutEvent */
    			timeoutsInUse = ( RXF_TimeoutEvent* )tempTmrPtr->event.next;
    			/* #MISRA_11.3=ON */
    		}
    		else 
    		{
    			tempTmrPtr->previous->event.next = tempTmrPtr->event.next;
    		}
    		
    		if( NULL != tempTmrPtr->event.next )
    		{
    			/* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_Event to RXF_TimeoutEvent */
    			((RXF_TimeoutEvent*)tempTmrPtr->event.next)->remainingTicks += tempTmrPtr->remainingTicks;
    			((RXF_TimeoutEvent*)tempTmrPtr->event.next)->previous = tempTmrPtr->previous;
    			/* #MISRA_11.3=ON */
    		}
    		RXF_TimerManager_putBackTimer(&tempTmrPtr->event);
    	}	
    	tempTmrPtr = next;	
    }
    RXF_CRITICALSECTION_UNLOCK_CALL
}

