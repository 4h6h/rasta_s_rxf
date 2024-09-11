/*
 * RXF_Startup 
 *
 * This file belongs to the Willert Software Tools RXF Product  
 * RXF_Rpy_C.
 *
 * Copyright (c) 2023, Willert Software Tools GmbH.
 *
 * E-mail: support(at)willert.de
 */


#include "RXF_Startup.h"
#include "RXF_Config.h"
#include "RXF_EventQueue.h"
#include "RXF_HighWaterMarks.h"
#include "RXF_MainTask.h"
#include "RXF_MemoryManager.h"
#include "RXF_Reactive.h"
#include "RXF_Tick.h"
#include "RXF_TimerManager.h"
/*## package selectable::RTOS::COORTX */

/*## class TopLevel::RXF_Startup */
bool RXF_Startup_Init(void)
{
    RXF_TimerManager_Init();
    RXF_MemoryManager_Init();
    RXF_EventQueue_Init( &mainMSQ );
    
    #ifdef RXF_HIGH_WATER_MARKS_ENABLED
    RXF_HighWaterMarks_Init();       
    #endif /* RXF_HIGH_WATER_MARKS_ENABLED */
     
    RXF_Tick_Init();
    
    return true;
}

