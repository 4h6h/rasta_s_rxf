#include "main.h"
#include <RXF_EntryPoint.h>

#include "SandboxOperation.h"

#include "cFec.h"
#include "cDisp.h"
#include "cRastaS.h"
#include "sm.h"
#include "RastaS_Cfg.h"

#include "assert.h"
#include "log.h"
#include "isc_cfg.h"

static SmType sms[MAX_CONNECTIONS] = { 0 };

static cFecOP itsCFecOP;
static cDispOP itsCDispOP;
static cRastaSOP itsCRastaSOP;

extern RastaSConfig rastaConfig_client;

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



/*## class TopLevel::main */
/*## operation main(int,char**) */
int main(int argc, char** argv) {
/*#[ operation main(int,char**) */
    rastaConfig_client.sms = sms;
    SandboxInstances instances = {
        .itsCFecOP = &itsCFecOP,
        .itsCDispOP = &itsCDispOP,
        .itsCRastaSOP = &itsCRastaSOP,
        .rastaConfig = &rastaConfig_client,
    };

    RXF_EntryPoint_Init(&instances);


    /* server loop forever, to be replaced by benis server loop */
    uint32_t cycle = 0;
    while (messages[cycle].ptr_to_payload != NULL) 
    {
        switch (messages[cycle].isc_message_source) {
            case MSG_FROM_RASTA_R:
            {
                LOG_INFO("Message size %lu", HEARTBEAT_MESSAGE_SIZE);
                Rass_ReceiveSpdu(itsCDispOP.p_Interface.outBound._iDispRastaS, 0, HEARTBEAT_MESSAGE_SIZE, messages[cycle].ptr_to_payload);
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
                Rass_Main(itsCDispOP.p_Interface.outBound._iDispRastaS);
                Fec_Main(itsCDispOP.p_Interface.outBound._iDispFec);
                
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

    return EXIT_SUCCESS;
}