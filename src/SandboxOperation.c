#include <RXF_MainTask.h>

#include "RXF_StandardTypes.h"
#include "SandboxOperation.h"
#include "cFec.h"
#include "cDisp.h"
#include "cRastaS.h"
#include "RastaS_Cfg.h"

static cFecOP* itsCFecOP;
static cDispOP* itsCDispOP;
static cRastaSOP* itsCRastaSOP;
static RastaSConfig* rastaConfig;

static void SandboxOperation_initRelations(void) {
    cFecOP_Init(itsCFecOP);
    cDispOP_Init(itsCDispOP);
    cRastaSOP_Init(itsCRastaSOP, rastaConfig);

    cFecOP_p_Interface_connectOutBound( itsCFecOP, itsCRastaSOP->p_Interface.inBound._iFec );

    cDispOP_p_Interface_connectOutBound_RastaS( itsCDispOP, itsCRastaSOP->p_Interface.inBound._iDisp);
    cDispOP_p_Interface_connectOutBound_Fec( itsCDispOP, itsCFecOP->p_Interface.inBound._iDisp );
    
    cRastaSOP_p_Interface_connectOutBound_Fec( itsCRastaSOP, itsCFecOP->p_Interface.inBound._iRastaS );
    cRastaSOP_p_Interface_connectOutBound_Disp( itsCRastaSOP, itsCDispOP->p_Interface.inBound._iRastaSDisp );
}

static RiCBoolean SandboxOperation_startBehavior(void) {
    RiCBoolean done = RiCTRUE;
    done = cFecOP_startBehavior(itsCFecOP) && done;
    done = cDispOP_startBehavior(itsCDispOP) && done;
    done = cRastaSOP_startBehavior(itsCRastaSOP) && done;
    return done;
}

static void SandboxOperation_OMInitializer_Init(void) {
    SandboxOperation_initRelations();
    (void) SandboxOperation_startBehavior();
}


void SandboxOperation_Main(void) {
   RXF_TimerManager_tick();
   RXF_TimerManager_processTimeouts();
   RXF_MainTask_executeAllEvents();
}

int32_t RXF_MainTask_start(SandboxInstances* instances) {

    int32_t status = NOT_OK;
    if(RiCOXFInit(argc, argv, 6423, "RXF_Startup", 0, 0, RiCTRUE)) {
        itsCFecOP = instances->itsCFecOP;
        itsCDispOP = instances->itsCDispOP;
        itsCRastaSOP = instances->itsCRastaSOP;
        rastaConfig = instances->rastaConfig;

        SandboxOperation_OMInitializer_Init();

        status = OK;
    }
    else {
        status = NOT_OK;
    }
    
    return status;
}