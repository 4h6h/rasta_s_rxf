/*
 * RXF_HighWaterMarks 
 *
 * This file belongs to the Willert Software Tools RXF Product  
 * RXF_Rpy_C.
 *
 * Copyright (c) 2023, Willert Software Tools GmbH.
 *
 * E-mail: support(at)willert.de
 */


#include "RXF_HighWaterMarks.h"
/*## package generic */

/*## class TopLevel::RXF_HighWaterMarks */

#ifdef RXF_HIGH_WATER_MARKS_ENABLED



/** 
 *  Local instance of the object.
 */ 
struct RXF_HighWaterMarks_t RXF_HighWaterMarks;
/*## operation Init() */
void RXF_HighWaterMarks_Init(void)
{
    RXF_HighWaterMarks.blockOverflow = false;
    RXF_HighWaterMarks.timerInUseCount = 0U;
    RXF_HighWaterMarks.timerInUseMax = 0U;
    {
        #if (RXF_MEMORY_TINY_BLOCK_COUNT > 0U)
        RXF_HighWaterMarks.tinyBlockCount = 0U;
        RXF_HighWaterMarks.tinyBlockMax = 0U;
        #endif
        #if (RXF_MEMORY_SMALL_BLOCK_COUNT > 0U)
        RXF_HighWaterMarks.smallBlockCount = 0U;
        RXF_HighWaterMarks.smallBlockMax = 0U;  
        #endif
        #if (RXF_MEMORY_MEDIUM_BLOCK_COUNT > 0U)
        RXF_HighWaterMarks.mediumBlockCount = 0U;
        RXF_HighWaterMarks.mediumBlockMax = 0U;  
        #endif
        #if (RXF_MEMORY_LARGE_BLOCK_COUNT > 0U)
        RXF_HighWaterMarks.largeBlockCount = 0U;
        RXF_HighWaterMarks.largeBlockMax = 0U;
        #endif
        #if (RXF_MEMORY_HUGE_BLOCK_COUNT > 0U)
        RXF_HighWaterMarks.hugeBlockCount = 0U;
        RXF_HighWaterMarks.hugeBlockMax = 0U;
        #endif
    
    }
}

#endif /* RXF_HIGH_WATER_MARKS_ENABLED */


