#include "cFec.h"
#include "log.h"

/*## class cFec */
static void rootState_entDef(void * const void_me);
static RXFTakeEventStatus rootState_dispatchEvent(void * const void_me, const RXFEventId id);
static void rootState_handleNotConsumed(RXF_Reactive* const me, RXF_Event* event);

void cFecOP_Init(cFecOP* const me) {
     static const struct RXF_Reactive_Vtbl cFecSOP_reactiveVtbl = {
        rootState_dispatchEvent,
        rootState_entDef,
        rootState_handleNotConsumed,
    };

    static const struct iRastaS_Fec_Vtbl cFecOP_iRastaSFec_Vtbl_Values = {
        (StdRet_t (*)(struct iRastaSFec* const me, const ConnId_t connId, const MsgLen_t msgLen, const uint8_t* const pMsgData)) cFecOP_ReceiveMsg,
        (StdRet_t (*)(struct iRastaSFec* const me, const ConnId_t connId, const struct RastaSConn* const state)) cFecOP_ConnStateNotification,
    };
    
    
    /* Initilize interface iPing. */
    /* #MISRA_11.3,11.5=OFF XD-0000: cast from specific class type to generic Interface type */
    me->_RastaSFec.realMe = (iRastaSFec*)me;
    /* #MISRA_11.3,11.5=ON */
    me->_RastaSFec.iRastaSFecVtbl = &cFecOP_iRastaSFec_Vtbl_Values;
    me->p_Interface.inBound._iRastaS = &me->_RastaSFec;
    RXF_Reactive_init(&(me->ric_reactive), &cFecSOP_reactiveVtbl);

}

void cFecOP_Cleanup(cFecOP* const me) {
}

RiCBoolean cFecOP_startBehavior(cFecOP* const me) {
    RiCBoolean done;
    done = RXF_Reactive_startBehavior(&(me->ric_reactive));
    return done;
}


static void rootState_entDef(void * const void_me) {};
static RXFTakeEventStatus rootState_dispatchEvent(void * const void_me, const RXFEventId id)
{
    RXFTakeEventStatus res = eventConsumed;
    cFecOP * const me = (cFecOP *)void_me;

    return res;
};
static void rootState_handleNotConsumed(RXF_Reactive* const me, RXF_Event* event){};


/*## operations */
StdRet_t cFecOP_ReceiveMsg(cFecOP* const me, const ConnId_t connId, const MsgLen_t msgLen, const uint8_t* const pMsgData) {
    StdRet_t returnValue = OK;
    /*#[*/

    /*#]*/
    return returnValue;
}

StdRet_t cFecOP_ConnStateNotification(cFecOP* const me, ConnId_t connId, const struct RastaSConn* const state) {
    StdRet_t returnValue = OK;

    return returnValue;
}


cFecOP * cFecOP_Create(void) {
    cFecOP* me = (cFecOP *) RXF_MemoryManager_getMemory(sizeof(cFecOP));
    if(me!=NULL)
        {
            cFecOP_Init(me);
        }
    DYNAMICALLY_ALLOCATED(me);
    return me;
}

void cFecOP_Destroy(cFecOP* const me) {
    if(me!=NULL)
        {
            cFecOP_Cleanup(me);
        }
    RXF_MemoryManager_returnMemory(me);
}


void cFecOP_p_Interface_connectOutBound(cFecOP* const me, iFec* const p_iFec) {
    if( NULL != p_iFec )
    {
    	me->p_Interface.outBound._iFec = p_iFec;
    }
}




void cFecOP_Main(cFecOP* const me) {

    // Rass_CloseConnection(OPORT(cFecOP, p_Interface, iFec), 0);
    // Rass_OpenConnection(OPORT(cFecOP, p_Interface, iFec), 0);

#if 0
    RastaSConn connState;
    const uint8_t data[32] = { 0 };

    Rass_CloseConnection(OPORT(cFecOP, p_Interface, iFec), 0);
    Rass_ConnectionStateRequest(OPORT(cFecOP, p_Interface, iFec), 0, &connState);
    Rass_SendDate(OPORT(cFecOP, p_Interface, iFec), 0, sizeof(data), data);

    Rass_CloseConnection(itsCFecOP.p_Interface.outBound._iFec->realMe, 0U);

#endif
}