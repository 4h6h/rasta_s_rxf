
#include "RXF_CriticalSection.h"
#include "RXF_HighWaterMarks.h"
#include "RXF_ErrorHandler.h"
#include "RXF_EventQueue.h"
#include "RXF_Event.h"
#include "RXF_TimeoutEvent.h"

#include "RXF_SM_Event.h"

static struct RXF_SM_Event* SMEventsInUse = NULL;
static struct RXF_SM_Event* firstSMEvent = NULL;

extern RXF_EventQueue mainMSQ;
/* Memory for all timer objects */ 
struct RXF_SM_Event smPool[RXF_MAX_SM_EVENTS];


void RXF_SM_Init(void)
{    
    uint32_t i;
    for( i = 0U; i < ( RXF_MAX_SM_EVENTS - 1U ) ; i++)
    {
    	/* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_TimeoutEvent to RXF_Event */
    	smPool[ i ].event.next = (RXF_Event*)&smPool[ i + 1U ];
    	/* #MISRA_11.3=ON */
    	smPool[ i ].event.isStatic = true;
    	smPool[ i ].event.lId = (RXFEventId)RXF_Event_Sm_id;
    }
    smPool[ RXF_MAX_SM_EVENTS - 1U ].event.next = NULL;
    smPool[ RXF_MAX_SM_EVENTS - 1U ].event.isStatic = true;
    smPool[ RXF_MAX_SM_EVENTS - 1U ].event.lId = (RXFEventId)RXF_Event_Sm_id;
    firstSMEvent = &smPool[ 0U ];
}


static void RXF_SM_processEvents()
{
    RXF_SM_Event* smEvent = NULL;

    while ( ( SMEventsInUse != NULL )  )
    {
    	RXF_CRITICALSECTION_VARIABLE
    	RXF_CRITICALSECTION_LOCK_CALL
    		
    	smEvent = SMEventsInUse;
    	
    	/* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_Event to RXF_TimeoutEvent */
    	SMEventsInUse = (RXF_SM_Event*)smEvent->event.next;
    	/* #MISRA_11.3=ON */
    	
    	RXF_CRITICALSECTION_UNLOCK_CALL

        // smEvent->event.destination = &(me->ric_reactive);
        RXF_EventQueue_put( &mainMSQ, &smEvent->event );

    }
}

void RXF_SM_Event_gen(RXF_Reactive* dest, ConnId_t conn_id, Event event) {
    RXF_SM_Event* smEvent;

     RXF_CRITICALSECTION_VARIABLE

     RXF_CRITICALSECTION_LOCK_CALL

    if( firstSMEvent != NULL )
    {
        smEvent = firstSMEvent;
    	/* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_Event to RXF_TimeoutEvent */
    	firstSMEvent = (RXF_SM_Event*)firstSMEvent->event.next;
    	/* #MISRA_11.3=ON */
    	
    	RXF_CRITICALSECTION_UNLOCK_CALL

        smEvent->conn_id = conn_id;
        smEvent->sm_event = event;
        smEvent->event.destination = dest;

        if ( SMEventsInUse == NULL )
        {
            /* The list was empty so we place it in the list.
    
                        Before: ->NULL
                        New:    [150]
                        After:  [150]->NULL
                */
            
            SMEventsInUse = smEvent;
            smEvent->event.next = NULL;
            smEvent->previous = NULL;	
        }
        else
        {
            RXF_SM_Event *tempPtr;
            /* No it must be after the first element. Good, now we only have to
                    walk acros the list. Now let tempTmrPtr point to the first element
                */
            
            tempPtr = SMEventsInUse;

            /* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_Event to RXF_TimeoutEvent */
            while(tempPtr->event.next != NULL)
            {
                tempPtr = (RXF_SM_Event*)tempPtr->event.next;
            }
            /* #MISRA_11.3=ON */
            
            /* OK, we found the element where we place the new one after
                    This can be the last, the last element's next pointer is NULL
                    but we can handle that.
                */
            smEvent->event.next = tempPtr->event.next;
            smEvent->previous = tempPtr;
            /* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_TimeoutEvent to RXF_Event */
            tempPtr->event.next = (RXF_Event*)smEvent;
            /* #MISRA_11.3=ON */
            if( smEvent->event.next != NULL )
            {
                /* #MISRA_11.3=OFF ID-0001: Pointercast from RXF_Event to RXF_TimeoutEvent */
                ((RXF_SM_Event*)smEvent->event.next)->previous = smEvent;
                /* #MISRA_11.3=ON */
            }
        }
        RXF_CRITICALSECTION_UNLOCK_CALL	
        RXF_SM_processEvents();
    } 
    else 
    {
        RXF_CRITICALSECTION_UNLOCK_CALL
    	RXF_ErrorHandler_error( RXF_ERROR_REACTIVE_EMPTY_EVENT_PTR, 0, true );
    }
}
