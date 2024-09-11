/*
 * RXF_EntryPoint 
 *
 * This file belongs to the Willert Software Tools RXF Product  
 * RXF_Rpy_C.
 *
 * Copyright (c) 2023, Willert Software Tools GmbH.
 *
 * E-mail: support(at)willert.de
 */


#include "RXF_EntryPoint.h"
#include "RXF_MainTask.h"
/*## package selectable::Main::False */

/*## class TopLevel::RXF_EntryPoint */
/*## operation RXF_EntryPoint_Init() */
void RXF_EntryPoint_Init(struct SandboxInstances* instances)
{
    (void)RXF_MainTask_start(instances);
}

