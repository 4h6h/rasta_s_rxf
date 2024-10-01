#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>

#include <cmocka.h>

#include <RXF_EntryPoint.h>
#include <RXF_MainTask.h>

#include "SandboxOperation.h"

#include "cFec.h"
#include "cDisp.h"
#include "cRastaS.h"
#include "sm.h"
#include "RastaS_Cfg.h"

#include "log.h"
#include "isc_cfg.h"

static SmType sms[MAX_CONNECTIONS] = { 0 };

static cFecOP itsCFecOP;
static cDispOP itsCDispOP;
static cRastaSOP itsCRastaSOP;

static uint8_t from_rasta_r_conn_resp_message[] = { 
    0x00, 0x32, /* Message length = 50 */
    0x18, 0x39, /* (RastaS) Message type = 6201 */
    0x00, 0x06, 0x45, 0x82, /* Receiver ID */
    0x00, 0x06, 0x45, 0x83, /* Sender ID */
    0x00, 0x00, 0x12, 0x67, /* Sequence number */
    0x00, 0x00, 0x12, 0x66, /* Confirmed sequence number */
    0xAA, 0x55, 0xAA, 0x55, /* Timestamp */
    0x55, 0xAA, 0x55, 0xAA, /* Confirmed timestamp */
    0x30, 0x33, 0x30, 0x31, /* Version of the protocol */
    0x00, 0x01, /* Size of the receive buffer in messages */
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, /* reserved */
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00 /* Safety code */
};
#define CONN_RESP_MESSAGE_SIZE sizeof(from_rasta_r_conn_resp_message)/sizeof(uint8_t)

static uint8_t from_rasta_r_conn_req_message[] = { 
    0x00, 0x32, /* Message length = 50 */
    0x18, 0x38, /* (RastaS) Message type = 6200 */
    0x00, 0x06, 0x45, 0x82, /* Receiver ID */
    0x00, 0x06, 0x45, 0x83, /* Sender ID */
    0x00, 0x00, 0x12, 0x67, /* Sequence number */
    0x00, 0x00, 0x12, 0x66, /* Confirmed sequence number */
    0xAA, 0x55, 0xAA, 0x55, /* Timestamp */
    0x55, 0xAA, 0x55, 0xAA, /* Confirmed timestamp */
    0x30, 0x33, 0x30, 0x31, /* Version of the protocol */
    0x00, 0x01, /* Size of the receive buffer in messages */
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, /* reserved */
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00 /* Safety code */
};
#define CONN_REQ_MESSAGE_SIZE sizeof(from_rasta_r_conn_req_message)/sizeof(uint8_t)

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

static void test_sm_client_open_up(void** state)
{
    RastaSConfig rastaConfig = {
        .instname = "client\0",
        .max_connections = MAX_CONNECTIONS,
        .role = ROLE_CLIENT,
    };
    const uint32_t channel = 0;
    rastaConfig.sms = sms;
    SandboxInstances instances = {
        .itsCFecOP = &itsCFecOP,
        .itsCDispOP = &itsCDispOP,
        .itsCRastaSOP = &itsCRastaSOP,
        .rastaConfig = &rastaConfig,
    };

    RXF_EntryPoint_Init(&instances);
    assert_true(instances.rastaConfig->sms[channel].state == STATE_CLOSED);
    
    Rass_OpenConnection(itsCFecOP.p_Interface.outBound._iFec, channel);
    RXF_MainTask_executeAllEvents();
    assert_true(instances.rastaConfig->sms[channel].state == STATE_START);
    
    Rass_ReceiveSpdu(itsCDispOP.p_Interface.outBound._iDispRastaS, channel, CONN_RESP_MESSAGE_SIZE, from_rasta_r_conn_resp_message);
    RXF_MainTask_executeAllEvents();
    assert_true(instances.rastaConfig->sms[channel].state == STATE_UP);

}

static void test_sm_server_open_up(void** state) {
    RastaSConfig rastaConfig = {
        .instname = "server\0",
        .max_connections = MAX_CONNECTIONS,
        .role = ROLE_SERVER,
    };
    const uint32_t channel = 0;
    rastaConfig.sms = sms;
    SandboxInstances instances = {
        .itsCFecOP = &itsCFecOP,
        .itsCDispOP = &itsCDispOP,
        .itsCRastaSOP = &itsCRastaSOP,
        .rastaConfig = &rastaConfig,
    };

    RXF_EntryPoint_Init(&instances);
    assert_true(instances.rastaConfig->sms[channel].state == STATE_CLOSED);

    Rass_OpenConnection(itsCFecOP.p_Interface.outBound._iFec, channel);
    RXF_MainTask_executeAllEvents();
    assert_true(instances.rastaConfig->sms[channel].state == STATE_DOWN);

    Rass_ReceiveSpdu(itsCDispOP.p_Interface.outBound._iDispRastaS, channel, CONN_REQ_MESSAGE_SIZE, from_rasta_r_conn_req_message);
    RXF_MainTask_executeAllEvents();
    assert_true(instances.rastaConfig->sms[channel].state == STATE_START);

    Rass_ReceiveSpdu(itsCDispOP.p_Interface.outBound._iDispRastaS, channel, HEARTBEAT_MESSAGE_SIZE, from_rasta_r_heartbeat_message);
    RXF_MainTask_executeAllEvents();
    assert_true(instances.rastaConfig->sms[channel].state == STATE_UP);
}

extern int test_sm_connection(void) {
    int return_value = NOT_OK;

    set_loglevel_filter(LOG_LEVEL);
    printf("LOG_LEVEL: %s\n", LOG_STRING[get_loglevel_filter()]);

    const struct CMUnitTest sm_connection_tests[] = {
        // cmocka_unit_test(test_sm_server_open_up),
        cmocka_unit_test(test_sm_client_open_up),
    };

    return_value = cmocka_run_group_tests_name("sm_connection_tests", sm_connection_tests, NULL, NULL);

    return return_value;
}