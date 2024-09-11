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

#include "log.h"

#include "RXF_MainTask.h"
#include "RXF_CriticalSection.h"
#include "RXF_ErrorHandler.h"
#include "RXF_MemoryManager.h"
#include "RXF_Reactive.h"
#include "RXF_TimerManager.h"
/*## package selectable::RTOS::COORTX */

/*## class TopLevel::RXF_MainTask */
RXF_EventQueue mainMSQ;

/*## operation RXF_MainTask_executeAllEvents() */
/* #MISRA_8.7=OFF ID-0005: This operation will be called by the customer only in special cases. */   
void RXF_MainTask_executeAllEvents(void)
{
	LOG_INFO ("RXF_MainTask_executeAllEvents");
    while( true == RXF_MainTask_executeOneEvent() )
    {
    	
    }
}
/* #MISRA_8.7=ON (end of ID-0005) */

/*## operation RXF_MainTask_executeOneEvent() */
/* #MISRA_8.7=OFF ID-0005: This operation will be called by the customer only in special cases. */   
bool RXF_MainTask_executeOneEvent(void)
{
    RXF_Event* event;
    bool retVal = false;
    
    RXF_CRITICALSECTION_VARIABLE
    
    RXF_MONITOR_EXECUTE_ONCE
    
    RXF_CRITICALSECTION_LOCK_CALL
    
    event = RXF_EventQueue_get( &mainMSQ );
    
    RXF_CRITICALSECTION_UNLOCK_CALL

	LOG_INFO("RXF_MainTask_executeEvent");
    
    if ( NULL != event )
    {
    	if ( NULL == event->destination )
    	{
    		/* Description: Detected RXF_ERROR_EVENT_NULL_DEST. */
    		/* #MISRA_11.4,11.6=OFF ID-0009: cast from pointer to int32_t only for the error handler */
    		/* RXF_ErrorHandler_error( RXF_ERROR_EVENT_DESTINATION_NULL , (int_t)event, true ); */
			RXF_ErrorHandler_error( RXF_ERROR_EVENT_DESTINATION_NULL , (uintptr_t)event, true );
    		/* #MISRA_11.4,11.6=ON */
    	}
    	else
    	{
    		/* Description: Send the event to its receiver's FSM. */
    		RXF_Reactive_takeEvent( event->destination, event );
    		retVal = true;
    	}
    	
    	if( false == event->isStatic )
    	{
    		RXF_MemoryManager_returnMemory( event );
    	}
    	else if( (RXFEventId)RXF_Event_Timeout_id == event->lId )
    	{
    		RXF_TimerManager_putBackTimer( event );
    	}
    	else
    	{
    		event->destination = NULL;
    	}
    }
    
    return retVal;
}
/* #MISRA_8.7=ON (end of ID-0005) */

