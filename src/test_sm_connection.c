#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>

#include <cmocka.h>

#include <RXF_EntryPoint.h>

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

extern RastaSConfig rastaConfig_client;

static void test_sm_client_open(void** state)
{
    uint32_t cycles = CYCLES;
    rastaConfig_client.sms = sms;
    SandboxInstances instances = {
        .itsCFecOP = &itsCFecOP,
        .itsCDispOP = &itsCDispOP,
        .itsCRastaSOP = &itsCRastaSOP,
        .rastaConfig = &rastaConfig_client,
    };

    RXF_EntryPoint_Init(&instances);
    
    Rass_OpenConnection(itsCFecOP.p_Interface.outBound._iFec, 0);

    while (cycles) 
    {
        SandboxOperation_Main();
        --cycles;
    }
}

extern int test_sm_connection(void) {
    int return_value = NOT_OK;

    set_loglevel_filter(LOG_LEVEL);
    printf("LOG_LEVEL: %s\n", LOG_STRING[get_loglevel_filter()]);

    const struct CMUnitTest sm_connection_tests[] = {
        cmocka_unit_test(test_sm_client_open),
    };

    return_value = cmocka_run_group_tests_name("sm_connection_tests", sm_connection_tests, NULL, NULL);

    return return_value;
}