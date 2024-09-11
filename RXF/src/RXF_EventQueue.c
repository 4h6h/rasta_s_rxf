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


#include "RXF_EventQueue.h"
#include "RXF_Event.h"
#include "RXF_MemoryManager.h"
#include "RXF_Reactive.h"
#include "RXF_TimerManager.h"
/*## package selectable::RTOS::COORTX */

/*## class RXF_EventQueue */
void RXF_EventQueue_Init(RXF_EventQueue* const me)
{
    me->head = NULL;
    me->tail = NULL;
}

void RXF_EventQueue_cancelEvents(RXF_EventQueue* const me, const struct RXF_Reactive* const destination)
{
    RXF_Event* parent;
    RXF_Event* event;
    
    parent = NULL;
    event = me->head;
    
    /* Now check every element */
    while( NULL != event )
    {
    	if( destination == event->destination )
    	{
    		RXF_Event* tempNextEvent;
    		
    		if(parent == NULL)
    		{
    			me->head = event->next;
    			if(me->head == NULL)
    			{
    				me->tail = NULL;
    			}
    		}
    		else
    		{
    			parent->next = event->next;
    			if( event == me->tail ){
    				me->tail = parent;
    			}
    		}
    		tempNextEvent = event->next;
    		
    		if( event->isStatic == false )
    		{
    			RXF_MemoryManager_returnMemory( event );
    		}
    		event = tempNextEvent;
    	}
    	else
    	{
    		parent = event;
    		event = event->next;
    	}
    }
}

void RXF_EventQueue_cancelTimeout(RXF_EventQueue* const me, const struct RXF_Reactive* const destination, const RXF_TimeoutEvent_ID_t timerID)
{
    RXF_Event* parent;
    RXF_Event* event;
    
    parent = NULL;
    event = me->head;
    
    /* Now check every element */
    while( NULL != event )
    {
    	/* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_Event to RXF_TimeoutEvent */   
    	if( ( destination == event->destination ) && 
    	    ( (RXFEventId)RXF_Event_Timeout_id == event->lId ) && 
    	    ( timerID == ((RXF_TimeoutEvent*)event)->timer_id ) )
    	/* #MISRA_11.3=ON */
    	{
    		if(parent == NULL)
    		{
    			me->head = event->next;
    			if(me->head == NULL)
    			{
    				me->tail = NULL;
    			}
    		}
    		else
    		{
    			parent->next = event->next;
    			if( event == me->tail ){
    				me->tail = parent;
    			}
    		}
    		RXF_TimerManager_putBackTimer( event );
    		break;
    		
    	}
    	else
    	{
    		parent = event;
    		event = event->next;
    	}
    }
}

struct RXF_Event* RXF_EventQueue_get(RXF_EventQueue* const me)
{
    RXF_Event* event = NULL; 
    
    if( !RXF_EventQueue_isEmpty( me ) )
    {
    	event = me->head; 
    	me->head = event->next;
    	if( NULL == me->head )
    	{
    		me->tail = NULL;
    	}
    }
    	
    return event;
}

bool RXF_EventQueue_isEmpty(const RXF_EventQueue* const me)
{
    return ( NULL == me->head );
}

void RXF_EventQueue_put(RXF_EventQueue* const me, struct RXF_Event* const event)
{
    event->next = NULL;
    
    if( NULL == me->tail )
    {
    	me->head = event;
    }
    else
    {
    	me->tail->next = event;
    }
    
    me->tail = event;
}

