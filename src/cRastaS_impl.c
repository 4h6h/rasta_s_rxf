#include "RXF_CriticalSection.h"
#include "RXF_HighWaterMarks.h"
#include "RXF_ErrorHandler.h"
#include "RXF_EventQueue.h"
#include "RXF_Event.h"
#include "RXF_TimeoutEvent.h"

#include "cRastaS_impl.h"
#include "RXF_SM_Event.h"
#include "assert.h"
#include "sm.h"
#include "log.h"

static SmType* sms = NULL;

RXFTakeEventStatus rootState_dispatchEvent_impl(void* const void_me, const RXFEventId id) {

    RXFTakeEventStatus res = eventNotConsumed;
    cRastaSOP * const me = (cRastaSOP *)void_me;
    
    LOG_INFO("rootState_dispatchEvent_impl: %i", id);

    switch((int16_t)id) {
        case RXF_Event_Timeout_id:
        {
            /* handle timouts */
            RXF_TimeoutEvent_ID_t tid = RiCTimeout_getTimeoutId((RiCTimeout*) me->ric_reactive.current_event);
            LOG_INFO("rootState_dispatchEvent_impl RXF_Event_timeout_id timeout id: %u", (uint16_t)tid);
            RXF_TimerManager_stop(tid, &(me->ric_reactive));

            res = eventConsumed;
            break;
        };
        case RXF_Event_Sm_id:
        {
            /* handle my own SM state machine events */
            RXF_SM_Event* ev = (RXF_SM_Event*)me->ric_reactive.current_event;
            ConnId_t conn_id = (ConnId_t)((RXF_SM_Event*)ev)->conn_id;
            Event sm_event = ((RXF_SM_Event*)ev)->sm_event;

            LOG_INFO("rootState_dispatchEvent_impl RXF_Event_sm_id conn_id: %u sm_event: %u", conn_id, sm_event);

            PDU_S pdu = { 0 };
            Sm_HandleEvent(&sms[conn_id], sm_event, &pdu);
        
            res = eventConsumed;
            break;
        };
        default:
            /* the rest */
            assert(0);
            break;
    }
    return res;
}


void cRastaSOP_Init_impl(cRastaSOP* const me, const struct RastaSConfig* const pConfig) {

    assert(me != NULL);
    assert(pConfig != NULL);
 
    /* Implementation specific to SafeCom_Init */
    LOG_INFO("init module %s in role %i", pConfig ->instname, pConfig->role);

    RXF_SM_Init();

    sms = pConfig->sms;

    for (int i=0; i<pConfig->max_connections; i++) {
        sms[i].channel = i;
        sms[i].state = STATE_CLOSED;
        sms[i].role = pConfig->role;
        sms[i].category = CAT_IGNORE_EVENT;
        Sm_Init(&sms[i]);
    }
};

StdRet_t cRastaSOP_OpenConnection_impl(cRastaSOP* const me, const ConnId_t conn_id) {

    StdRet_t resultValue = OK;

    assert(me != NULL);

    RXF_SM_Event_gen(&(me->ric_reactive), conn_id, EVENT_OPEN_CONN);

    RXF_TimerManager_start(10, 42, &(me->ric_reactive));

    return resultValue;
};


StdRet_t cRastaSOP_CloseConnection_impl(cRastaSOP* const me, const ConnId_t conn_id) {

    StdRet_t resultValue = OK;

    assert(me != NULL);

    RXF_SM_Event_gen(&(me->ric_reactive), conn_id, EVENT_CLOSE_CONN);    
    
    return resultValue;
};
