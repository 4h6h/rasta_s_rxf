#include "main.h"
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

/*## class TopLevel::main */
/*## operation main(int,char**) */
int main(int argc, char** argv) {
/*#[ operation main(int,char**) */
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

    return EXIT_SUCCESS;
}