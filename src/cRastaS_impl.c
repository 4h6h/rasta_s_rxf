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

RXFTakeEventStatus rootState_dispatchEvent_impl(void* const void_me, const RXFEventId id) {

    RXFTakeEventStatus res = eventNotConsumed;
    cRastaSOP* const me = (cRastaSOP *)void_me;
    
    LOG_INFO("rootState_dispatchEvent_impl: %i", id);

    switch((int16_t)id) {
        case RXF_Event_Timeout_id:
        {
            /* handle timouts */
            RXF_TimeoutEvent_ID_t tid = RiCTimeout_getTimeoutId((RiCTimeout*) me->ric_reactive.current_event);
            LOG_INFO("rootState_dispatchEvent_impl RXF_Event_timeout_id timeout id: %u", (uint16_t)tid);
            ConnId_t conn_id = (ConnId_t)tid;

            PDU_S pdu;
            Sm_HandleEvent(&me->sms[conn_id], EVENT_TI_ELAPSED, &pdu);
            if (pdu.message_length > 0)
            {
                uint8_t* send_data = (uint8_t*)RXF_MemoryManager_getMemory(pdu.message_length);
                serialize_pdu(&pdu, send_data, pdu.message_length);
                RXF_SM_Event_gen(&(me->ric_reactive), RXF_Event_Sm_out_id, conn_id, EVENT_NO_SM_EVENT, send_data);
                
            }

            RXF_TimerManager_stop(tid, &(me->ric_reactive));
            res = eventConsumed;
            break;
        };
        case RXF_Event_Sm_in_id:
        {
            /* handle my own SM state machine events incoming */
            RXF_SM_Event* ev = (RXF_SM_Event*)me->ric_reactive.current_event;
            ConnId_t conn_id = (ConnId_t)((RXF_SM_Event*)ev)->conn_id;
            Event sm_event = ((RXF_SM_Event*)ev)->sm_event;
            const uint8_t* data = ((RXF_SM_Event*)ev)->data;
            State old_state = me->sms[conn_id].state;

            LOG_INFO("rootState_dispatchEvent_impl RXF_Event_sm_in_id conn_id: %u sm_event: %u", conn_id, sm_event);
    
            PDU_S pdu = { 0 }; /* pointer to payload and safety code - no buffer provided */
            if (data == NULL) { /* connection interface */
                Sm_HandleEvent(&me->sms[conn_id], sm_event, &pdu);

                if (pdu.message_length > 0)
                {
                    uint8_t* send_data = (uint8_t*)RXF_MemoryManager_getMemory(pdu.message_length);
                    serialize_pdu(&pdu, send_data, pdu.message_length);
                    RXF_SM_Event_gen(&(me->ric_reactive), RXF_Event_Sm_out_id, conn_id, EVENT_NO_SM_EVENT, send_data);
                }
            } else 
            { /* everything else */
                size_t dataLength =  (size_t)((data[0] & 0xFF)<<8 | (data[1] & 0xFF));
                deserialize_pdu(data, dataLength, &pdu);
                Sm_HandleEvent(&me->sms[conn_id], sm_event, &pdu);

                if (pdu.message_length > 0)
                {
                    LOG_INFO("Response %u %u", pdu.message_length, pdu.message_type);
                    uint8_t* send_data = (uint8_t*)RXF_MemoryManager_getMemory(pdu.message_length);
                    serialize_pdu(&pdu, send_data, pdu.message_length);
                    RXF_SM_Event_gen(&(me->ric_reactive), RXF_Event_Sm_out_id, conn_id, EVENT_NO_SM_EVENT, send_data);

                    if (pdu.message_type==MSGT_HEARTBEAT)
                    {
                        LOG_INFO("A new HB leads to timer reset");
                        RXF_TimerManager_start(CYCLE_TICKS, conn_id, &(me->ric_reactive));
                    }

                }
            }

            if (old_state != me->sms[conn_id].state)
            {
                RastaSConn* conn_state = (RastaSConn*)RXF_MemoryManager_getMemory(sizeof(RastaSConn));
                conn_state->old = old_state;
                conn_state->new = me->sms[conn_id].state;
                RXF_SM_Event_gen(&(me->ric_reactive), RXF_Event_Sm_state_id, conn_id, EVENT_NO_SM_EVENT, (uint8_t*)conn_state);
            }

            // RXF_SM_Event_free(ev);
        
            res = eventConsumed;
            break;
        };
        case RXF_Event_Sm_out_id:
        {
            /* handle my own SM state machine events outgoing */
            RXF_SM_Event* ev = (RXF_SM_Event*)me->ric_reactive.current_event;
            ConnId_t conn_id = (ConnId_t)((RXF_SM_Event*)ev)->conn_id;
            Event sm_event = ((RXF_SM_Event*)ev)->sm_event;
            const uint8_t* data = ((RXF_SM_Event*)ev)->data;

            LOG_INFO("rootState_dispatchEvent_impl RXF_Event_sm_out_id conn_id: %u sm_event: %u", conn_id, sm_event);
            
            size_t dataLength =  (size_t)((data[0] & 0xFF)<<8 | (data[1] & 0xFF));
            Disp_SendSpdu(OPORT(me, p_Interface, iRastaSDisp), 0, dataLength, data);

            // RXF_SM_Event_free(ev);

            res = eventConsumed;
            break;
        };
        case RXF_Event_Sm_state_id:
        {
            RXF_SM_Event* ev = (RXF_SM_Event*)me->ric_reactive.current_event;
            ConnId_t conn_id = (ConnId_t)((RXF_SM_Event*)ev)->conn_id;
            Event sm_event = ((RXF_SM_Event*)ev)->sm_event;
            const RastaSConn* conn_state = (RastaSConn*)(((RXF_SM_Event*)ev)->data);

            LOG_INFO("rootState_dispatchEvent_impl RXF_Event_sm_state_id conn_id: %u sm_event: %u", conn_id, sm_event);
            Fec_ConnStateNotfication(OPORT(me, p_Interface, iRastaSFec), 0, conn_state);

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

    me->sms = pConfig->sms;

    for (int i=0; i<pConfig->max_connections; i++) {
        me->sms[i].channel = i;
        me->sms[i].state = STATE_CLOSED;
        me->sms[i].role = pConfig->role;
        me->sms[i].category = CAT_IGNORE_EVENT;
        Sm_Init(&me->sms[i]);
    }
};

StdRet_t cRastaSOP_OpenConnection_impl(cRastaSOP* const me, const ConnId_t conn_id) {

    StdRet_t resultValue = OK;

    assert(me != NULL);

    RXF_SM_Event_gen(&(me->ric_reactive), RXF_Event_Sm_in_id, conn_id, EVENT_OPEN_CONN, NULL);

    /* RXF_TimerManager_start(10, 42, &(me->ric_reactive)); */

    return resultValue;
};


StdRet_t cRastaSOP_CloseConnection_impl(cRastaSOP* const me, const ConnId_t conn_id) {

    StdRet_t resultValue = OK;

    assert(me != NULL);

    RXF_SM_Event_gen(&(me->ric_reactive), RXF_Event_Sm_in_id, conn_id, EVENT_CLOSE_CONN, NULL);    
    
    return resultValue;
};


StdRet_t cRastaSOP_ReceiveSpdu_impl(cRastaSOP* const me, const ConnId_t conn_id, const SpduLen_t spduLen, const uint8_t* const pSpduData) {

    StdRet_t resultValue = OK;

    LOG_INFO("spduLen %u %x %x %x %x", spduLen, pSpduData[0], pSpduData[1], pSpduData[2], pSpduData[3]);

    assert(me != NULL);
    assert(spduLen > 0);
    assert(pSpduData != NULL);
    assert((SpduLen_t)(((pSpduData[0] & 0xFF)<<8) | (pSpduData[1] & 0xFF)) == spduLen);

    uint16_t message_type = (uint16_t)(((pSpduData[2] & 0xFF)<<8) | (pSpduData[3] & 0xFF) );

    switch (message_type) {
        case MSGT_HEARTBEAT:
        {
            RXF_SM_Event_gen(&(me->ric_reactive), RXF_Event_Sm_in_id, conn_id, EVENT_RECV_HB, pSpduData);
            break;
        }
        case MSGT_CONNECTION_RESPONSE:
        {
            RXF_SM_Event_gen(&(me->ric_reactive), RXF_Event_Sm_in_id, conn_id, EVENT_RECV_CONN_RESP, pSpduData);
            break;
        }
        case MSGT_CONNECTION_REQUEST:
        {
            RXF_SM_Event_gen(&(me->ric_reactive), RXF_Event_Sm_in_id, conn_id, EVENT_RECV_CONN_REQ, pSpduData);
            break;
        }
        default:
            /* Never get here */
            assert(0);
            break;
    }  
    
    return resultValue;
};

