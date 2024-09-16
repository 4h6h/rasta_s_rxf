#include <RXF_EntryPoint.h>

#include "cFec.h"
#include "cDisp.h"
#include "cRastaS.h"
#include "sm.h"
#include "RastaS_Cfg.h"

#include "assert.h"
#include "log.h"
#include "isc_cfg.h"
#include "SandboxOperation.h"


/* */
typedef enum {
    MSG_FROM_RASTA_R = 0U,
    MSG_FROM_ISC_X,
    MSG_FROM_TIMER,
} IscMessageSource;

static uint8_t from_rasta_r_heartbeat_message[] = { 
    0x00, 0x24, /* Message length = 36 */
    0x18, 0x4C, /* (RastaS) Message type = 6220 */
    0x00, 0x06, 0x45, 0x82, /* Receiver ID */
    0x00, 0x06, 0x45, 0x83, /* Sender ID */
    0x00, 0x00, 0x12, 0x67, /* Sequence number */
    0x00, 0x00, 0x12, 0x66, /* Confirmed sequence number */
    0xAA, 0x55, 0xAA, 0x55, /* Timestamp */
    0x55, 0xAA, 0x55, 0xAA, /* Confirmed timestamp */
                            /* no payload */
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00 /* Safety code */
};

#define HEARTBEAT_MESSAGE_SIZE sizeof(from_rasta_r_heartbeat_message)/sizeof(uint8_t)

static uint8_t from_timer_timing_message[] = {
    0x5A, 0x5A, 0x5A, 0x5A,  /* Timestamp */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* Safety code */
};

typedef struct {
    const IscMessageSource isc_message_source;
    const uint8_t* const ptr_to_payload;
} IscMessage;


static IscMessage messages[] = {
    {
        .isc_message_source = MSG_FROM_TIMER,
        .ptr_to_payload = (void *)&from_timer_timing_message,
    },
    {
        .isc_message_source = MSG_FROM_RASTA_R,
        .ptr_to_payload = (void *)&from_rasta_r_heartbeat_message,
    },
    {
        .isc_message_source = MSG_FROM_TIMER,
        .ptr_to_payload = (void *)&from_timer_timing_message,
    },
    {
        .isc_message_source = MSG_FROM_TIMER,
        .ptr_to_payload = (void *)&from_timer_timing_message,
    },
    {
        .isc_message_source = MSG_FROM_TIMER,
        .ptr_to_payload = (void *)&from_timer_timing_message,
    },
    {
        0
    }
};


static SmType sms[MAX_CONNECTIONS] = { 0 };

static cDispOP istCDispOP;
static cFecOP itsCFecOP;
static cRastaSOP itsCRastaSOP;
extern RastaSConfig rastaConfig_client;



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


int32_t modelMain(void) {
    rastaConfig_client.sms = sms;
    SandboxInstances instances = {
        .itsCDispOP = &istCDispOP,
        .itsCFecOP = &itsCFecOP,
        .itsCRastaSOP = &itsCRastaSOP,
        .rastaConfig = &rastaConfig_client,
    };

    RXF_EntryPoint_Init(&instances);

    cDispOP_Main(&istCDispOP);
    return OK;
}


void cDispOP_Main(cDispOP* const me) 
{
    /* server loop forever, to be replaced by benis server loop */
    uint32_t cycle = 0;
    while (messages[cycle].ptr_to_payload != NULL) 
    {
        switch (messages[cycle].isc_message_source) {
            case MSG_FROM_RASTA_R:
            {
                LOG_INFO("Message size %lu", HEARTBEAT_MESSAGE_SIZE);
                Rass_ReceiveSpdu(me->p_Interface.outBound._iDispRastaS, 0, HEARTBEAT_MESSAGE_SIZE, messages[cycle].ptr_to_payload);
                break;
            }
            case MSG_FROM_ISC_X:
            {
                /* handle the incoming messages form the other isc instance in order to compare results */
                break;
            }
            case MSG_FROM_TIMER:
            {
                /* process one timer message to process real time value */
                /* realtime =  { 0x5A, 0x5A, 0x5A, 0x5A }; */
                
                /* process all main functions ? in a specific order ? */
                Rass_Main(me->p_Interface.outBound._iDispRastaS);
                Fec_Main(me->p_Interface.outBound._iDispFec);
                
                /* process RXF step - one tick */
                SandboxOperation_Main();
                break;
            }
            default:
                /* never get here */
                assert(0);
                break;

        }
        cycle++;
    }

}