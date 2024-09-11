#include "main.h"
#include "log.h"
#include "SandboxOperation.h"

#include <RXF_Config.h>
#include <RXF_EntryPoint.h>
#include <RXF_MainTask.h>
#include <RXF_TimerManager.h>
#include <RXF_VirtualTable.h>
#include <stdlib.h>
#include <unistd.h>



/*## class TopLevel::main */
/*## operation main(int,char**) */
int _main(int argc, char** argv) {
/*#[ operation main(int,char**) */
    // RXF_EntryPoint_Init();

    while (true)
    {
        /* SandboxOperation_Main(); */
    }

    return EXIT_SUCCESS;
}