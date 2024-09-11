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


#include "RXF_Reactive.h"
#include "RXF_Config.h"
#include "RXF_CriticalSection.h"
#include "RXF_ErrorHandler.h"
#include "RXF_EventQueue.h"
#include "RXF_MainTask.h"
#include "RXF_MemoryManager.h"
#include "RXF_TimerManager.h"
#include "RXF_VirtualTable.h"
/*## package selectable::RTOS::COORTX */

/*## class RXF_Reactive */

/** 
 *  Handles run-to-completion behavior by calling dispatchEvent() for null transitions.
 *  Returns true if successful, false if infinite loop is detected that has to be solved another way.
 */ 
/**
 *  Satisfies requirement 2891: Reactive - Infinite Null Transitions
 *  Reactive shall detect cyclic null transitions and call the ErrorHandler in such a case.
 *  
 *  Satisfies requirement 2890: Reactive - Run To Completion
 *  Reactive shall fulfill the run to completion behavior of a statechart.
 *  
 */
/*## operation runToCompletion() */
static void runToCompletion(RXF_Reactive* const me);


/**
 *  Returns true, if the nullTransitionCount is > 0.
 */ 
static inline bool inNullConfig(const RXF_Reactive* const me);


/**
 *  Returns true if the object is in started status, false otherwise.
 */ 
static inline bool isBehaviorStarted(const RXF_Reactive* const me);


/**
 *  Returns true if the object is in busy status, false otherwise.
 */ 
static inline bool isBusy(const RXF_Reactive* const me);


/**
 *  Sets if the object is in started status.
 */ 
static inline void setBehaviorStarted(RXF_Reactive* const me);

/*## operation gen(RXF_Event) */
void RXF_Reactive_gen(RXF_Reactive* const me, struct RXF_Event* const event RXF_MONITOR_FSM_GEN_SOURCE_ARGUMENT)
{
    if ( NULL == me )
    {
    	RXF_ErrorHandler_error( RXF_ERROR_EMPTY_ME_PTR, 0, true );
    }
    else if ( NULL == event )
    {
    	/* #MISRA_11.4,11.6=OFF ID-0009: cast from pointer to int32_t only for the error handler */
    	/* RXF_ErrorHandler_error( RXF_ERROR_REACTIVE_EMPTY_EVENT_PTR, (int_t)me, true ); */
		RXF_ErrorHandler_error( RXF_ERROR_REACTIVE_EMPTY_EVENT_PTR, (uintptr_t)(me), true );
    	/* #MISRA_11.4,11.6=ON */
    }
    else
    {
    	if ( (NULL != event->destination) && ( (RXFEventId)RXF_Event_Timeout_id != event->lId ) )
    	{
    		/* #MISRA_11.4,11.6=OFF ID-0009: cast from pointer to int32_t only for the error handler */
    		/* RXF_ErrorHandler_error( RXF_ERROR_REACTIVE_EVENT_REUSE_DETECTED, (int_t)event, true ); */
			RXF_ErrorHandler_error( RXF_ERROR_REACTIVE_EVENT_REUSE_DETECTED, (uintptr_t)event, true );
    		/* #MISRA_11.4,11.6=ON */
    	}
    	else
    	{
    		RXF_CRITICALSECTION_VARIABLE
    		
    		/* Set the destination for the event */
    		event->destination = me;
    		
    		RXF_MONITOR_SEND_EVENT_CREATE
    		
    		
        	RXF_CRITICALSECTION_LOCK_CALL
    		
    		/* put it in the queue */
    		RXF_EventQueue_put( &mainMSQ, event );
    		
    		RXF_CRITICALSECTION_UNLOCK_CALL
    	}
    }
}

/*## operation startBehavior() */
bool RXF_Reactive_startBehavior(RXF_Reactive* const me)
{
    bool returnValue = false;      
    
    /* Check the me pointer */
    if ( NULL == me )
    {
    	RXF_ErrorHandler_error( RXF_ERROR_EMPTY_ME_PTR, 0, true );
    }
    /* Check the vtbl pointer */
    else if ( NULL == me->vtbl )
    {
    	/* #MISRA_11.4,11.6=OFF ID-0009: cast from pointer to int32_t only for the error handler */
    	/* RXF_ErrorHandler_error( RXF_ERROR_REACTIVE_EMPTY_VTBL_PTR, (int_t)me, true ); */
		RXF_ErrorHandler_error( RXF_ERROR_REACTIVE_EMPTY_VTBL_PTR, (uintptr_t)me, true );
    	/* #MISRA_11.4,11.6=ON */
    }
    else
    {
    	/* start behavior only once */
    	if ( false == isBehaviorStarted(me) )
    	{
    		/* mark that startBehavior was called */
    		setBehaviorStarted( me );
            
    		/* There is no else path, as it is a normal use case that Rhapsody
    		 * generates a NULL-pointer entry for entDefFunc e.g. when a class
    		 * does not have a statemachine, but contains parts that implement
    		 * state behavior.
    		 */
    		if ( NULL != me->vtbl->rootState_entDefFunc )
    		{
    			/* Protect from recursive triggered operation calls */
    			me->status |= RXF_Reactive_statusBusy;	/* busy flag is true */ 
    			(*me->vtbl->rootState_entDefFunc)(me);
    
    			/* Generate a dummy event in case the instance doesn't receive */
    			/* any external events. This causes the runToCompletion()	   */
    			/* operation to be taken after the default transition.		   */
    			if ( false != inNullConfig(me) )
    			{     
    				runToCompletion(me);
    			}
    			me->status &= RXF_Reactive_statusBusyInverted;	/* busy flag is false */
    		}
    		returnValue = true;
    	}
    	else
    	{
    		/* #MISRA_11.4,11.6=OFF ID-0009: cast from pointer to int32_t only for the error handler */
    		/* RXF_ErrorHandler_error( RXF_ERROR_REACTIVE_RESTART_DETECTED, (int_t)me, true ); */
			RXF_ErrorHandler_error( RXF_ERROR_REACTIVE_RESTART_DETECTED, (uintptr_t)me, true );
    		/* #MISRA_11.4,11.6=ON */
    	}
    }
    
    return returnValue;
}

/*## operation runToCompletion() */
static void runToCompletion(RXF_Reactive* const me)
{
    /* No me != NULL, me->vtbl, me->vtbl->dispEventFunc pointer check needed, because it is static and just called after pointer check. */
    static RXF_Event 		nullEvent;
    uint16_t				nullStepCount;
    RXFTakeEventStatus		status;
    RXFDispatchEvent		dispatchEvent;
    bool					stopNullSteps;
    
    nullStepCount		= 0;
    stopNullSteps		= false;
    dispatchEvent		= me->vtbl->dispEventFunc;
    me->current_event	= &nullEvent;
    nullEvent.lId		= (RXFEventId)RXF_Event_Null_id;
    
    /* If untriggered transitions are "hot" take them */
    while ( false != inNullConfig(me) )
    {
    
    	nullStepCount++;
    	if ( nullStepCount > RXF_REACTIVE_MAX_NULL_STEPS )
    	{
    		/* Found infinite loop */
    		stopNullSteps = true;
    
    		/* #MISRA_11.4,11.6=OFF ID-0009: cast from pointer to int32_t only for the error handler */
    		/* RXF_ErrorHandler_error ( RXF_ERROR_INFINITE_NULL_TRANSITIONS, (int_t)me, true ); */
			RXF_ErrorHandler_error ( RXF_ERROR_INFINITE_NULL_TRANSITIONS, (uintptr_t)me, true );
    		/* #MISRA_11.4,11.6=ON */
    
    		/* reset the null config count */
    		me->nullTransitionCount = 0U;
    	}
    	else
    	{
    		status = dispatchEvent( me, (RXFEventId)RXF_Event_Null_id );
    
    		/* Perform the null transition */
    		if ( ( eventNotConsumed == status ) )
    		{
    			stopNullSteps = true;
    		}
    	}
    	if( stopNullSteps )
    	{
    		break;
    	}
    }
}

/*## operation init(RXF_Reactive_Vtbl) */
void RXF_Reactive_init(RXF_Reactive* const me, const struct RXF_Reactive_Vtbl* vtbl)
{
    if ( NULL == me )
    {
    	RXF_ErrorHandler_error( RXF_ERROR_EMPTY_ME_PTR, 0, true );
    }
    else if ( NULL == vtbl )
    {
    	/* #MISRA_11.4,11.6=OFF ID-0009: cast from pointer to int32_t only for the error handler */
    	/* RXF_ErrorHandler_error( RXF_ERROR_REACTIVE_EMPTY_VTBL_PTR, (int_t)me, true ); */
		RXF_ErrorHandler_error( RXF_ERROR_REACTIVE_EMPTY_VTBL_PTR, (uintptr_t)me, true ); 
    	/* #MISRA_11.4,11.6=ON */
    }
    else
    {
    	me->vtbl				= vtbl;
    	me->current_event		= NULL;
    	me->status				= RXF_Reactive_statusDefault;
    	me->nullTransitionCount	= 0U; 
    }
    
}

/*## operation consumeGivenEvent(RXF_Event) */
/* #MISRA_8.7=OFF ID-0003: Operation needs to be visible by the TestConductor when used. */   
void RXF_Reactive_consumeGivenEvent(RXF_Reactive* const me, struct RXF_Event* const event)
{
    RXFDispatchEvent dispatchEvent;
    RXFTakeEventStatus dispatchEventResult;
    
    /* No NULL pointer check for the me-pointer and the vtbl pointer required here, as
     * only takeEvent() calls this private operation and already perform the check.
     * Also the event pointer does not need to be checked for a NULL pointer, as
     * takeEvent() operation is only called from executeEvent in RXF_Startup, where
     * the event check has already been performed or from Rhapsody generated triggered
     * operation code, where the event object is a local variable (on stack).
     */
    
    dispatchEvent = me->vtbl->dispEventFunc;
    if ( NULL != dispatchEvent )
    {
    	me->current_event = event;
    	
    	RXF_MONITOR_SEND_EVENT_START
    
    	/* handle the event */
    	dispatchEventResult = dispatchEvent( me, event->lId );
    		
    	if( dispatchEventResult != eventConsumed )
    	{
    		/* Timeout events may still occur after state changes caused by events which were in
    		 * the event queue before the timeout. They are state specific and can be ignored
    		 * when the state has changed in between.
    		 */
    		if( (RXFEventId)RXF_Event_Timeout_id != event->lId )
    		{
    #ifdef RXF_VTBL_HANDLE_EVENT_NOT_CONSUMED
    			if( NULL != me->vtbl->handleEventNotConsumed )
    			{
    				me->vtbl->handleEventNotConsumed( me, event );
    			}
    			else
    			{
    				/* #MISRA_11.4,11.6=OFF ID-0009: cast from pointer to int32_t only for the error handler */
    				/* RXF_ErrorHandler_error( RXF_ERROR_REACTIVE_EVENT_NOT_CONSUMED, (int_t)me, true ); */
					RXF_ErrorHandler_error( RXF_ERROR_REACTIVE_EVENT_NOT_CONSUMED, (uintptr_t)me, true );
    				/* #MISRA_11.4,11.6=ON */
    			}
    #else /* RXF_VTBL_HANDLE_EVENT_NOT_CONSUMED */
    			/* #MISRA_11.4,11.6=OFF ID-0009: cast from pointer to int32_t only for the error handler */
    			/* RXF_ErrorHandler_error( RXF_ERROR_REACTIVE_EVENT_NOT_CONSUMED, (int_t)me, true ); */
				RXF_ErrorHandler_error( RXF_ERROR_REACTIVE_EVENT_NOT_CONSUMED, (uintptr_t)me, true ); 
    			/* #MISRA_11.4,11.6=ON */
    #endif /* RXF_VTBL_HANDLE_EVENT_NOT_CONSUMED */
    		}
    	}
    
    	/* check runToCompletion() */
    	if ( false != inNullConfig(me) )
    	{
    		runToCompletion(me);
    	}
    	
    	me->current_event = NULL;
    	
    	RXF_MONITOR_SEND_EVENT_END
    	
    	me->status &= RXF_Reactive_statusBusyInverted;	/* busy flag is false */
    }
}
/* #MISRA_8.7=ON (end of ID-0003) */

/*## operation cleanup() */
/* #MISRA_8.7=OFF ID-0004: This operation will be called by the user code but only when the user allows reactive destruction. */   
void RXF_Reactive_cleanup(const RXF_Reactive* const me)
{
    RXF_CRITICALSECTION_VARIABLE 
       						
    RXF_CRITICALSECTION_LOCK_CALL
    RXF_EventQueue_cancelEvents( &mainMSQ, me );
    RXF_CRITICALSECTION_UNLOCK_CALL
    
    RXF_TimerManager_stopDestination( me );
}
/* #MISRA_8.7=ON (end of ID-0004) */

/*## operation cancelTimeout(RXF_TimeoutEvent_ID_t) const */
void RXF_Reactive_cancelTimeout(const RXF_Reactive* const me, const RXF_TimeoutEvent_ID_t timerID)
{
    RXF_CRITICALSECTION_VARIABLE
        						
    RXF_CRITICALSECTION_LOCK_CALL
    RXF_EventQueue_cancelTimeout( &mainMSQ, me, timerID );
    RXF_CRITICALSECTION_UNLOCK_CALL
}

void RXF_Reactive_terminate(const RXF_Reactive* const me)
{
    RXF_Reactive_cleanup( me );
}

static inline bool inNullConfig(const RXF_Reactive* const me)
{
    /* No me != NULL pointer check needed, because it is static and just called from static function
     * runToCompletion(), which is just called after me pointer check.
     */
    return (me->nullTransitionCount > 0U );
}

static inline bool isBehaviorStarted(const RXF_Reactive* const me)
{
    /* No me != NULL pointer check needed, because it is static and just called after me pointer check.
     */
    bool ret;
    
    if ( (me->status & RXF_Reactive_statusStarted) != RXF_Reactive_statusFALSE )
    {
    	ret = true;
    }
    else
    {
    	ret = false;
    }
    return ret;
}

static inline bool isBusy(const RXF_Reactive* const me)
{
    /* No me != NULL pointer check needed, because it is static and just called after me pointer check.
     */
    bool ret;
    
    if ( (me->status & RXF_Reactive_statusBusy ) != RXF_Reactive_statusFALSE )
    {
    	ret = true;
    }
    else
    {
    	ret = false;
    }
    return ret;
}

static inline void setBehaviorStarted(RXF_Reactive* const me)
{
    me->status |= RXF_Reactive_statusStarted;
}

void RXF_Reactive_popNullConfig(RXF_Reactive* const me)
{
    /* We do not need to check the me pointer, as takeEvent() or startBehavior() perform the me pointer check, calls
     * dispatchEvent of the user generated code and from only there this operation is called again. */
    if ( me->nullTransitionCount > 0U )
    {
    	me->nullTransitionCount--;
    }
    else
    {
    	/* #MISRA_11.4,11.6=OFF ID-0009: cast from pointer to int32_t only for the error handler */
    	/* RXF_ErrorHandler_error( RXF_ERROR_TOO_FEW_NULL_CONFIGS, (int_t)me, true ); */
		RXF_ErrorHandler_error( RXF_ERROR_TOO_FEW_NULL_CONFIGS, (uintptr_t)me, true );
    	/* #MISRA_11.4,11.6=ON */
    }
}

void RXF_Reactive_pushNullConfig(RXF_Reactive* const me)
{
    /* We do not need to check the me pointer, as takeEvent() or startBehavior() perform the me pointer check, calls
     * dispatchEvent of the user generated code and from only there this operation is called again. */
    me->nullTransitionCount++;
}

void RXF_Reactive_takeEvent(RXF_Reactive* const me, struct RXF_Event* const event)
{
    if ( NULL == me )
    {
    	RXF_ErrorHandler_error( RXF_ERROR_EMPTY_ME_PTR, 0, true );
    }
    else if ( NULL == me->vtbl )
    {
    	/* #MISRA_11.4,11.6=OFF ID-0009: cast from pointer to int32_t only for the error handler */
    	/* RXF_ErrorHandler_error( RXF_ERROR_REACTIVE_EMPTY_VTBL_PTR, (int_t)me, true ); */
		RXF_ErrorHandler_error( RXF_ERROR_REACTIVE_EMPTY_VTBL_PTR, (uintptr_t)me, true );
    	/* #MISRA_11.4,11.6=ON */
    }
    else
    {
    	/* Note that event needs not to be tested for NULL; 
    	 * - calling RXF_Startup_executeOnce() is testing for event pointer already 
    	 * - calling RXF_Active_executeOnce() is testing for event pointer already
    	 */
    	
    	#ifdef RXF_VTBL_CONSUME_EVENT
    	if ( me->vtbl->consumeEvent == NULL )
    	{
    		RXF_Reactive_consumeGivenEvent( me, event );
    	}
    	else
    	{
    		me->vtbl->consumeEvent( me, event );
    	}
    	#else /* RXF_VTBL_CONSUME_EVENT */
    	RXF_Reactive_consumeGivenEvent( me, event );
    	#endif /* RXF_VTBL_CONSUME_EVENT */
    }
}

void RXF_Reactive_takeTrigger(RXF_Reactive* const me, struct RXF_Event* const event)
{
    if ( NULL == me )
    {
    	RXF_ErrorHandler_error( RXF_ERROR_EMPTY_ME_PTR, 0, true );
    }
    else if ( NULL == event )
    {
    	/* #MISRA_11.4,11.6=OFF ID-0009: cast from pointer to int32_t only for the error handler */
    	/* RXF_ErrorHandler_error( RXF_ERROR_REACTIVE_EMPTY_EVENT_PTR, (int_t)me, true ); */
		RXF_ErrorHandler_error( RXF_ERROR_REACTIVE_EMPTY_EVENT_PTR, (uintptr_t)me, true );
    	/* #MISRA_11.4,11.6=ON */
    }
    else
    {
    
    	/* Note that the event pointer does not need to be checked:
    	 * the generated code of the calling triggered operation has allocated the event local on stack
    	 */
    	event->destination = me;
    	/* Protect from recursive triggered operation calls */
    	if( !isBusy(me) )
    	{
    		me->status |= RXF_Reactive_statusBusy;	/* busy flag is true */ 
    		RXF_Reactive_takeEvent( me, event );
    		me->status &= RXF_Reactive_statusBusyInverted;	/* busy flag is false */
    	}
    	else
    	{
    		/* #MISRA_11.4,11.6=OFF ID-0009: cast from pointer to int32_t only for the error handler */
    		/* RXF_ErrorHandler_error( RXF_ERROR_REACTIVE_RECURSIVE_TRIGGERED_OPERATION_CALL, (int_t)me, true ); */
			RXF_ErrorHandler_error( RXF_ERROR_REACTIVE_RECURSIVE_TRIGGERED_OPERATION_CALL, (uintptr_t)me, true );
    		/* #MISRA_11.4,11.6=ON */
    	}
    }	
}

