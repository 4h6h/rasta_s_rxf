#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include "cmocka.h"

#include <RXF_MainTask.h>
#include <RXF_TimerManager.h>
#include <RXF_Startup.h>
#include <RXF_PortMacros.h>

#include "sm.h"
#include "log.h"

#include "cFec.h"
#include "cDisp.h"
#include "cRastaS.h"
#include "cRastaS_impl.h"


static void init_relations(void);
static void mainloop_body(void);

static struct cFecOP itsCFecOP;
static struct cDispOP itsCDispOP;
static struct cRastaSOP itsCRastaSOP;

extern RastaSConfig rastaConfig_client;

static SmType sms[MAX_CONNECTIONS] = { 0 }; 


static void init_relations(void) {
    cFecOP_Init(&(itsCFecOP));
    cDispOP_Init(&(itsCDispOP));
    cRastaSOP_Init(&(itsCRastaSOP));
    cRastaSOP_Init_impl(&(itsCRastaSOP), &(rastaConfig_client));
    // itsCRastaSOP = cRastaSOP_Create(&(rastaConfig_client));

    cFecOP_p_Interface_connectOutBound( &itsCFecOP, itsCRastaSOP.p_Interface.inBound._iFec );
    cDispOP_p_Interface_connectOutBound( &itsCDispOP, itsCRastaSOP.p_Interface.inBound._iDisp );
    cRastaSOP_p_Interface_connectOutBound_Fec( &itsCRastaSOP, itsCFecOP.p_Interface.inBound._iRastaS );
    cRastaSOP_p_Interface_connectOutBound_Disp( &itsCRastaSOP, itsCDispOP.p_Interface.inBound._iRastaSDisp );
}

static RiCBoolean start_behavior(void) {
    RiCBoolean done = RiCTRUE;
    done = cFecOP_startBehavior(&itsCFecOP) && done;
    done = cDispOP_startBehavior(&itsCDispOP) && done;
    done = cRastaSOP_startBehavior(&itsCRastaSOP) && done;
    return done;
}


static void mainloop_body (void) {
   RXF_TimerManager_tick();
   RXF_TimerManager_processTimeouts();
   RXF_MainTask_executeAllEvents();
}


static void test_sm_client_open(void** state)
{
    rastaConfig_client.sms = sms;

    int32_t status = 0;
    if(RiCOXFInit(argc, argv, 6423, "RXF_Startup", 0, 0, RiCTRUE))
        {
            init_relations();
            start_behavior();
            /*#[ configuration RXF_SandboxOperation::VisualStudio */
            /*#]*/
            status = 0;
        }
    else
        {
            status = 1;
        }
    
    Rass_OpenConnection(itsCFecOP.p_Interface.outBound._iFec, 0);

    int i=30;
    while (i) 
    {
        mainloop_body();
        i--;
    }
}

extern int test_sm_connection(void) {
    int return_value = -1;

    const struct CMUnitTest sm_connection_tests[] = {
        cmocka_unit_test(test_sm_client_open),
    };

    return_value = cmocka_run_group_tests_name("sm_connection_tests", sm_connection_tests, NULL, NULL);

    return return_value;
}