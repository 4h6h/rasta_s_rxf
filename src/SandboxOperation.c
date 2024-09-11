#include <RXF_Config.h>
#include <RXF_EntryPoint.h>
#include <RXF_MainTask.h>
#include <RXF_TimerManager.h>
#include <RXF_VirtualTable.h>

#include "SandboxOperation.h"

#include "cFec.h"
#include "cDisp.h"
#include "cRastaS.h"

// #include "Ipc.h"

static struct cFecOP itsCFecOP;
static struct cDispOP itsCDispOP;
static struct cRastaSOP itsCRastaSOP;

struct RastaSConfig;
extern struct RastaSConfig rastaConfig;

void SandboxOperation_OMInitializer_Init(void) {
    SandboxOperation_initRelations();
    (void) SandboxOperation_startBehavior();

}

void SandboxOperation_initRelations(void) {
    cFecOP_Init(&(itsCFecOP));
    cDispOP_Init(&(itsCDispOP));
    cRastaSOP_Init(&(itsCRastaSOP), &(rastaConfig));

    cFecOP_p_Interface_connectOutBound( &itsCFecOP, itsCRastaSOP.p_Interface.inBound._iFec );
    cDispOP_p_Interface_connectOutBound( &itsCDispOP, itsCRastaSOP.p_Interface.inBound._iDisp );
    cRastaSOP_p_Interface_connectOutBound_Fec( &itsCRastaSOP, itsCFecOP.p_Interface.inBound._iRastaS );
    cRastaSOP_p_Interface_connectOutBound_Disp( &itsCRastaSOP, itsCDispOP.p_Interface.inBound._iRastaSDisp );
    
}

RiCBoolean SandboxOperation_startBehavior(void) {
    RiCBoolean done = RiCTRUE;

    /* done = Ipc_init(); */ 

    return done;
}

void SandboxOperation_Main(void) {
   RXF_TimerManager_tick();
   RXF_TimerManager_processTimeouts();
   RXF_MainTask_executeAllEvents();
}

int32_t RXF_MainTask_start() {
    SandboxOperation_OMInitializer_Init();
    return OK;
}