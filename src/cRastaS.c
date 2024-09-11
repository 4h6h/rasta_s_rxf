#include "cRastaS.h"
#include "cRastaS_impl.h"
#include "log.h"

static void rootState_entDef(void * const void_me);
static RXFTakeEventStatus rootState_dispatchEvent(void * const void_me, const RXFEventId id);
static void rootState_handleNotConsumed(RXF_Reactive* const me, RXF_Event* event);

/*## class cDisp */
void cRastaSOP_Init(cRastaSOP* const me, const struct RastaSConfig* const pConfig) {
    static const struct RXF_Reactive_Vtbl cRastaSOP_reactiveVtbl = {
        rootState_dispatchEvent,
        rootState_entDef,
        rootState_handleNotConsumed,
    };

    static const struct iFec_Vtbl cRastaSOP_iFec_Vtbl_Values = {
        (StdRet_t (*)(iFec* const me, const ConnId_t connId)) cRastaSOP_OpenConnection,
        (StdRet_t (*)(iFec* const me, const ConnId_t connId)) cRastaSOP_CloseConnection,
        (StdRet_t (*)(iFec* const me, const ConnId_t connId, struct RastaSConn* const state)) cRastaSOP_ConnectionStateRequest,
        (StdRet_t (*)(iFec* const me, const ConnId_t connId, const MsgLen_t msgLen, const uint8_t* const pMsgData)) cRastaSOP_SendData,
    };
    static const struct iDisp_Vtbl cRastaSOP_iDisp_Vtbl_Values = {
        (StdRet_t (*)(iDisp* const me, const ConnId_t connId, const SpduLen_t spduLen, const uint8_t* const pSpduData)) cRastaSOP_ReceiveSpdu,
    };

    /* Initilize interface iPing. */
    /* #MISRA_11.3,11.5=OFF XD-0000: cast from specific class type to generic Interface type */
    me->_iFec.realMe = (iFec*)me;
    me->_iDisp.realMe = (iDisp*)me;
    
    /* #MISRA_11.3,11.5=ON */
    me->_iFec.iFecVtbl = &cRastaSOP_iFec_Vtbl_Values;
    me->_iDisp.iDispVtbl = &cRastaSOP_iDisp_Vtbl_Values;
    
    me->p_Interface.inBound._iFec = &me->_iFec;
    me->p_Interface.inBound._iDisp = &me->_iDisp;

    RXF_Reactive_init(&(me->ric_reactive), &cRastaSOP_reactiveVtbl);
 
    cRastaSOP_Init_impl(me, pConfig);

}

void cRastaSOP_Cleanup(cRastaSOP* const me) {
    RXF_Reactive_cleanup(&(me->ric_reactive));
}


cRastaSOP * cRastaSOP_Create(const struct RastaSConfig* const pConfig) {
    cRastaSOP* me = (cRastaSOP *) RXF_MemoryManager_getMemory(sizeof(cRastaSOP));
    if(me!=NULL)
        {
            cRastaSOP_Init(me, pConfig);
        }
    DYNAMICALLY_ALLOCATED(me);
    return me;
}


void cRastaSOP_Destroy(cRastaSOP* const me) {
    if(me!=NULL)
        {
            cRastaSOP_Cleanup(me);
        }
    RXF_MemoryManager_returnMemory(me);
}


RiCBoolean cRastaSOP_startBehavior(cRastaSOP* const me) {
    RiCBoolean done;
    done = RXF_Reactive_startBehavior(&(me->ric_reactive));
    return done;
}


StdRet_t cRastaSOP_OpenConnection(cRastaSOP* const me, const ConnId_t connId) {
    return cRastaSOP_OpenConnection_impl(me, connId);
}

StdRet_t cRastaSOP_CloseConnection(cRastaSOP* const me, const ConnId_t connId) {
    return cRastaSOP_CloseConnection_impl(me, connId);;
}

StdRet_t cRastaSOP_ConnectionStateRequest(cRastaSOP* const me, const ConnId_t connId, struct RastaSConn* const state) {
    StdRet_t returnValue = OK;

    return returnValue;
}

StdRet_t cRastaSOP_SendData(cRastaSOP* const me, const ConnId_t connId, MsgLen_t msgLen, const uint8_t* const pMsgData) {
    StdRet_t returnValue = OK;

    return returnValue;
};

StdRet_t cRastaSOP_ReceiveSpdu(cRastaSOP* const me, const ConnId_t connId, SpduLen_t spduLen, const uint8_t* const pSpduData) {
    StdRet_t returnValue = OK;

    return returnValue;
};


void cRastaSOP_p_Interface_connectOutBound_Fec(cRastaSOP* const me, iRastaSFec* const p_iRastaSFec) {
    if( NULL != p_iRastaSFec )
    {
    	me->p_Interface.outBound._iRastaSFec = p_iRastaSFec;
    }
}

void cRastaSOP_p_Interface_connectOutBound_Disp(cRastaSOP* const me, struct iRastaSDisp* const p_iRastaSDisp) {
    if( NULL != p_iRastaSDisp )
    {
    	me->p_Interface.outBound._iRastaSDisp = p_iRastaSDisp;
    }
}


static void rootState_entDef(void * const void_me) {
    cRastaSOP * const me = (cRastaSOP*)void_me;

    // RXF_TimerManager_start( 200, 42, &(me->ric_reactive));
}

static RXFTakeEventStatus rootState_dispatchEvent(void * const void_me, const RXFEventId id) {
    return rootState_dispatchEvent_impl(void_me, id);
}


static void rootState_handleNotConsumed(RXF_Reactive* const me, RXF_Event* event)
{
    LOG_INFO("handle not consumed");
}

static bool is_initialized = false;
void cRastaSOP_Main(cRastaSOP* const me) {
#if 0
    RastaSConn connState;
    uint8_t spdu[32] = { 0 };

    Fec_ReceiveMsg(OPORT(me, p_Interface, iRastaS), 0, 0, NULL);
    Fec_ConnStateNotfication(OPORT(me, p_Interface, iRastaS), 0, &connState);

    Disp_SendSpdu(OPORT(me, p_Interface, iRastaSDisp), 0, sizeof(spdu), spdu);

    if (!is_initialized) {
        RXF_TimerManager_start( 2, 42, &(me->ric_reactive));
    }
#endif
}