#include <arpa/inet.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <RXF_EntryPoint.h>
#include <RXF_MainTask.h>

#include "cFec.h"
#include "cDisp.h"
#include "cRastaS.h"
#include "sm.h"
#include "RastaS_Cfg.h"

#include "assert.h"
#include "log.h"
#include "isc_cfg.h"
#include "SandboxOperation.h"

#include "server_Cfg.h"

/* */
typedef enum {
    ISC_MSG_FROM_PEER_1 = PEER_1_HASH,
    ISC_MSG_FROM_PEER_2 = PEER_2_HASH,
    ISC_MSG_FROM_PEER_3 = PEER_3_HASH,
    ISC_MSG_FROM_TIMER = TIMER_HASH,
    ISC_MSG_EXIT = EXIT_HASH,
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
        .isc_message_source = ISC_MSG_FROM_TIMER,
        .ptr_to_payload = (void *)&from_timer_timing_message,
    },
    {
        .isc_message_source = ISC_MSG_FROM_PEER_1,
        .ptr_to_payload = (void *)&from_rasta_r_heartbeat_message,
    },
    {
        .isc_message_source = ISC_MSG_FROM_TIMER,
        .ptr_to_payload = (void *)&from_timer_timing_message,
    },
    {
        .isc_message_source = ISC_MSG_FROM_TIMER,
        .ptr_to_payload = (void *)&from_timer_timing_message,
    },
    {
        .isc_message_source = ISC_MSG_FROM_TIMER,
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
extern RastaSConfig rastaConfig;

static int rx_sockfd = 0;
static int tx_sockfd = 0;
static char dest_ip[INET6_ADDRSTRLEN];
static struct sockaddr_in tx_sockaddr;

/*## class cDisp */
static void rootState_entDef(void * const void_me);
static RXFTakeEventStatus rootState_dispatchEvent(void * const void_me, const RXFEventId id);
static void rootState_handleNotConsumed(RXF_Reactive* const me, RXF_Event* event);

static StdRet_t server_init(cDispOP* me);
static StdRet_t server_loop(cDispOP* me);
static void *get_ip_addr(struct sockaddr *sa);

static void *get_ip_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

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
    static int tx_bytes = 0;
    
    LOG_INFO("cDispOP_SendSpdu %u %u %x %x", connId, spduLen, pSpduData[2], pSpduData[3]);

    if ((tx_bytes = sendto(tx_sockfd, pSpduData, spduLen, 0, (struct sockaddr *)&tx_sockaddr, sizeof(tx_sockaddr))) == -1) {
        LOG_ERROR("Failed to send packet: %s.\n", strerror(errno));
        close(tx_sockfd);
        returnValue = NOT_OK;
    }

    RXF_MemoryManager_returnMemory((void *)pSpduData);

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


static StdRet_t server_init(cDispOP* me)
{
    struct sockaddr_in local_address;
    int port = LISTEN_PORT;

    strncpy(dest_ip, PEER_1_IP, INET6_ADDRSTRLEN);

    /* Create an IPv4 UDP RX socket */
    if ((rx_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        printf("Cannot create UDP socket: %s.\n", strerror(errno));
        return NOT_OK;
    }

    /* Initialize local address */
    memset(&local_address, 0, sizeof(local_address));
    local_address.sin_family = AF_INET;
    local_address.sin_port = htons(port);
    local_address.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Bind socket to local address */
    if (bind(rx_sockfd, (struct sockaddr *)&local_address, sizeof(local_address)) == -1) {
        printf("Cannot bind socket: %s.\n", strerror(errno));
        close(rx_sockfd);
        return NOT_OK;
    }

    /* Create IPv4 UDP TX socket */
    if ((tx_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        printf("Cannot create UDP socket: %s.\n", strerror(errno));
        return NOT_OK;
    }

    /* Initialize TX socket address */
    memset(&tx_sockaddr, 0, sizeof(struct sockaddr_in));
    tx_sockaddr.sin_family = AF_INET;
    if ((inet_pton(tx_sockaddr.sin_family, dest_ip, &(tx_sockaddr.sin_addr)) == -1)) {
        printf("Failed to convert IP address to binary form: %s.\n", strerror(errno));
        close(tx_sockfd);
        return NOT_OK;
    }
    tx_sockaddr.sin_port = htons(port);

    LOG_INFO("Started ISC dispatcher, parameters:");
    LOG_INFO("----------------------------------------");
    LOG_INFO("%-25s %s", "My IP:", inet_ntoa(local_address.sin_addr));
    LOG_INFO("%-25s %s", "Peer IP:", dest_ip);
    LOG_INFO("%-25s %d", "Input port:", port);
    LOG_INFO("----------------------------------------");

    return OK;
}

static uint64_t hash(const uint8_t* str)
{
    uint64_t hash = 5381U;
    int32_t c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

StdRet_t server_loop(cDispOP* const me)
{
    int recv_bytes = 0;
    uint8_t recv_buf[RECV_BUFSIZE];

    struct sockaddr_storage origin_address;
    socklen_t orig_addr_len = sizeof(origin_address);
    char source[INET6_ADDRSTRLEN];

    Fec_Main(me->p_Interface.outBound._iDispFec);
    RXF_MainTask_executeAllEvents();

    /* Wait for packets */
    while (1) {
        if ((recv_bytes = recvfrom(rx_sockfd, recv_buf, RECV_BUFSIZE, 0, (struct sockaddr *)&origin_address, &orig_addr_len)) == -1) {
            printf("Error reading RX socket: %s.\n", strerror(errno));
            return NOT_OK;
        }

        const uint8_t* remote_ip = inet_ntop(origin_address.ss_family, get_ip_addr((struct sockaddr *)&origin_address),
            source, INET6_ADDRSTRLEN);

        LOG_INFO("got UDP packet: origin = %s, size = %d, data = %x %x", remote_ip, recv_bytes, recv_buf[0], recv_buf[1]);
    
        switch(hash(remote_ip))
        {
            case ISC_MSG_FROM_PEER_1:
            {
                uint8_t* pSpduData = &recv_buf[0];
                SpduLen_t spduLen = (SpduLen_t)(((recv_buf[0] & 0xFF) << 8) | (recv_buf[1] & 0xFF));

                Rass_ReceiveSpdu(me->p_Interface.outBound._iDispRastaS, 0, spduLen, pSpduData);
                RXF_MainTask_executeAllEvents();
                break;
            }
            case ISC_MSG_FROM_TIMER:
            {
                SandboxOperation_Main();
                break;
            }
            case ISC_MSG_EXIT:
            {
                return OK;
            }
            default:
            {
                printf("%s : %lu", remote_ip, hash(remote_ip));
                /* never get here */
                //assert(0);
                break;
            }
        }
 
    }

    return NOT_OK;
}


int32_t modelMain(void) {
    rastaConfig.sms = sms;
    SandboxInstances instances = {
        .itsCDispOP = &istCDispOP,
        .itsCFecOP = &itsCFecOP,
        .itsCRastaSOP = &itsCRastaSOP,
        .rastaConfig = &rastaConfig,
    };

    RXF_EntryPoint_Init(&instances);

#ifndef SERVER_SIMULATION
    (void)server_init(instances.itsCDispOP);
#endif /* SERVER_SIMULATION */

    cDispOP_Main(&istCDispOP);
    return OK;
}


void cDispOP_Main(cDispOP* const me) 
{

#if SERVER_SIMULATION
    /* server loop forever, to be replaced by benis server loop */

    uint32_t cycle = 0;
    while (messages[cycle].ptr_to_payload != NULL) 
    {
        switch (messages[cycle].isc_message_source)
        {
            case ISC_MSG_FROM_PEER_1:
            {
                LOG_INFO("Message size %lu", HEARTBEAT_MESSAGE_SIZE);
                Rass_ReceiveSpdu(me->p_Interface.outBound._iDispRastaS, 0, HEARTBEAT_MESSAGE_SIZE, messages[cycle].ptr_to_payload);
                RXF_MainTask_executeAllEvents();
                break;
            }
            case ISC_MSG_FROM_PEER_2:
            {
                /* handle the incoming messages form the other isc instance in order to compare results */
                break;
            }
            case ISC_MSG_FROM_PEER_3:
            {
                /* handle the incoming messages from IUZP */
                break;
            }
            case ISC_MSG_FROM_TIMER:
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
#else /* SERVER_SIMULATION */

    (void)server_loop(me);

#endif /* SERVER_SIMULATION */

}