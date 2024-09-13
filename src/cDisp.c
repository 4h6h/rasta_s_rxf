#include "cDisp.h"

/*## class cDisp */
static void rootState_entDef(void * const void_me);
static RXFTakeEventStatus rootState_dispatchEvent(void * const void_me, const RXFEventId id);
static void rootState_handleNotConsumed(RXF_Reactive* const me, RXF_Event* event);

void cDispOP_Init(cDispOP* const me) {
    static const struct RXF_Reactive_Vtbl cDispSOP_reactiveVtbl = {
        rootState_dispatchEvent,
        rootState_entDef,
        rootState_handleNotConsumed,
    };

    static const struct iRastaS_Disp_Vtbl cDispOP_iRastaSDisp_Vtbl_Values = {
        (StdRet_t (*)(struct iRastaSDisp* const me, const ConnId_t connId, const SpduLen_t spduLen, const uint8_t* const pSpduData)) cDispOP_SendSpdu,
    };
    
    /* Initilize interface iPing. */
    /* #MISRA_11.3,11.5=OFF XD-0000: cast from specific class type to generic Interface type */
    me->_RastaSDisp.realMe = (iRastaSDisp*)me;
    /* #MISRA_11.3,11.5=ON */
    me->_RastaSDisp.iRastaSDispVtbl = &cDispOP_iRastaSDisp_Vtbl_Values;
    me->p_Interface.inBound._iRastaSDisp = &me->_RastaSDisp;
    RXF_Reactive_init(&(me->ric_reactive), &cDispSOP_reactiveVtbl);
}

void cDispOP_Cleanup(cDispOP* const me) {
}

RiCBoolean cDispOP_startBehavior(cDispOP* const me) {
    RiCBoolean done;
    done = RXF_Reactive_startBehavior(&(me->ric_reactive));
    return done;
}


static void rootState_entDef(void * const void_me) {};
static RXFTakeEventStatus rootState_dispatchEvent(void * const void_me, const RXFEventId id)
{
    RXFTakeEventStatus res = eventNotConsumed;

    return res;
};
static void rootState_handleNotConsumed(RXF_Reactive* const me, RXF_Event* event){};

cDispOP * cDispOP_Create(void) {
    cDispOP* me = (cDispOP *) RXF_MemoryManager_getMemory(sizeof(cDispOP));
    if(me!=NULL)
        {
            cDispOP_Init(me);
        }
    DYNAMICALLY_ALLOCATED(me);
    return me;
}

void cDispOP_Destroy(cDispOP* const me) {
    if(me!=NULL)
        {
            cDispOP_Cleanup(me);
        }
    RXF_MemoryManager_returnMemory(me);
}

/*## operation SendSpdu */
StdRet_t cDispOP_SendSpdu(cDispOP* const me, const ConnId_t connId, const SpduLen_t spduLen, const uint8_t* const pSpduData) {
    StdRet_t returnValue = OK;
    /*#[*/
    /*#]*/
    return returnValue;
}


void cDispOP_p_Interface_connectOutBound_RastaS(cDispOP* const me, iDispRastaS* const p_iDispRastaS) {
    if( NULL != p_iDispRastaS )
    {
    	me->p_Interface.outBound._iDispRastaS = p_iDispRastaS;
    }
}

void cDispOP_p_Interface_connectOutBound_Fec(cDispOP* const me, iDispFec* const p_iDispFec) {
    if( NULL != p_iDispFec )
    {
    	me->p_Interface.outBound._iDispFec = p_iDispFec;
    }
}


void cDispOP_Main(cDispOP* const me) 
{
}