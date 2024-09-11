#ifndef SANDBOX_OPERATION_H
#define SANDBOX_OPERATION_H

#include "RXF_StandardTypes.h"
#include "RXF_RhapsodyTypes.h"
#include "RXF_MemoryManager.h"
#include "RXF_EventMacros.h"
#include "RXF_PortMacros.h"
#include "RXF_Monitor.h"

void SandboxOperation_OMInitializer_Init(void);

void SandboxOperation_initRelations(void);

RiCBoolean SandboxOperation_startBehavior(void);

void SandboxOperation_Main(void);

#endif /* SANDBOX_OPERATION_H */