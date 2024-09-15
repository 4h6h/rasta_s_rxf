
#include "RXF_CriticalSection.h"
#include "RXF_HighWaterMarks.h"
#include "RXF_ErrorHandler.h"
#include "RXF_EventQueue.h"
#include "RXF_Event.h"
#include "RXF_TimeoutEvent.h"
#include "RXF_MemoryManager.h"

#include "RXF_SM_Event.h"

extern RXF_EventQueue mainMSQ;

void RXF_SM_Event_gen(const RXF_Reactive* const dest, const ConnId_t conn_id, const Event event, const PDU_S* const pdu) {

    RXF_SM_Event* me = (RXF_SM_Event*) RXF_MemoryManager_getMemory(sizeof(RXF_SM_Event));
    if(me!=NULL)
    {
        me->conn_id = conn_id;
        me->sm_event = event;
        me->pdu = pdu;
        RXF_Event_init(&(me->event), RXF_Event_Sm_id, dest);

        RXF_EventQueue_put( &mainMSQ, &me->event );
    }
}

void RXF_SM_Event_free(const RXF_SM_Event* const ev) {
    if (ev!=NULL)
    {
        RXF_MemoryManager_returnMemory((FREE_ARGUMENT_TYPE)ev);
    }
}