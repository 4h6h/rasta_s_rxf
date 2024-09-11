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


#ifndef generic_RXF_HighWaterMarks_H
#define generic_RXF_HighWaterMarks_H

#include "RXF_StandardTypes.h"
#include "RXF_Config.h"
/*## package generic */

/*## class TopLevel::RXF_HighWaterMarks */

#ifdef RXF_HIGH_WATER_MARKS_ENABLED


/** 
 *  RXF Class RXF_HighWaterMarks
 *
 *  Object to handle the high water marks.
 *  It can be used to monitor the maximum usage of the five static memory pools.
 *  High water marks are only activated when RXF_HIGH_WATER_MARKS_ENABLED is defined.
 */ 
struct RXF_HighWaterMarks_t
{
    
    /** 
     *  Indicates if any memory pool runs out of memory.
     */ 
    bool blockOverflow;		/*## attribute blockOverflow */
    
    /** 
     *  Count variable of the huge memory blocks. Represents how many huge blocks are currently in use.
     */ 
    #if (RXF_MEMORY_HUGE_BLOCK_COUNT > 0U)
    uint16_t hugeBlockCount;
    #endif /* RXF_MEMORY_HUGE_BLOCK_COUNT > 0U */		/*## attribute hugeBlockCount */
    
    /** 
     *  Largest number of blocks from the huge memory pool that has been used during the program execution.
     */ 
    #if (RXF_MEMORY_HUGE_BLOCK_COUNT > 0U)
    uint16_t hugeBlockMax;
    #endif /* RXF_MEMORY_HUGE_BLOCK_COUNT > 0U */		/*## attribute hugeBlockMax */
    
    /** 
     *  Count variable of the large memory blocks. Represents how many large blocks are currently in use.
     */ 
    #if (RXF_MEMORY_LARGE_BLOCK_COUNT > 0U)
    uint16_t largeBlockCount;
    #endif /* RXF_MEMORY_LARGE_BLOCK_COUNT > 0U */		/*## attribute largeBlockCount */
    
    /** 
     *  Largest number of blocks from the large memory pool that has been used during the program execution.
     */ 
    #if (RXF_MEMORY_LARGE_BLOCK_COUNT > 0U)
    uint16_t largeBlockMax;
    #endif /* RXF_MEMORY_LARGE_BLOCK_COUNT > 0U */		/*## attribute largeBlockMax */
    
    /** 
     *  Count variable of the medium memory blocks. Represents how many medium blocks are currently in use.
     */ 
    #if (RXF_MEMORY_MEDIUM_BLOCK_COUNT > 0U)
    uint16_t mediumBlockCount;
    #endif /* RXF_MEMORY_MEDIUM_BLOCK_COUNT > 0U */		/*## attribute mediumBlockCount */
    
    /** 
     *  Largest number of blocks from the medium memory pool that has been used during the program execution.
     */ 
    #if (RXF_MEMORY_MEDIUM_BLOCK_COUNT > 0U)
    uint16_t mediumBlockMax;
    #endif /* RXF_MEMORY_MEDIUM_BLOCK_COUNT > 0U */		/*## attribute mediumBlockMax */
    
    /** 
     *  Count variable of the small memory blocks. Represents how many small blocks are currently in use.
     */ 
    #if (RXF_MEMORY_SMALL_BLOCK_COUNT > 0U)
    uint16_t smallBlockCount;
    #endif /* RXF_MEMORY_SMALL_BLOCK_COUNT > 0U */		/*## attribute smallBlockCount */
    
    /** 
     *  Largest number of blocks from the small memory pool that has been used during the program execution.
     */ 
    #if (RXF_MEMORY_SMALL_BLOCK_COUNT > 0U)
    uint16_t smallBlockMax;
    #endif /* RXF_MEMORY_SMALL_BLOCK_COUNT > 0U */		/*## attribute smallBlockMax */
    
    /** 
     */ 
    uint32_t timerInUseCount;		/*## attribute timerInUseCount */
    
    /** 
     */ 
    uint32_t timerInUseMax;		/*## attribute timerInUseMax */
    
    /** 
     *  Count variable of the tiny memory blocks. Represents how many tiny blocks are currently in use.
     */ 
    #if (RXF_MEMORY_TINY_BLOCK_COUNT > 0U)
    uint16_t tinyBlockCount;
    #endif /* RXF_MEMORY_TINY_BLOCK_COUNT > 0U */		/*## attribute tinyBlockCount */
    
    /** 
     *  Largest number of blocks from the tiny memory pool that has been used during the program execution.
     */ 
    #if (RXF_MEMORY_TINY_BLOCK_COUNT > 0U)
    uint16_t tinyBlockMax;
    #endif /* RXF_MEMORY_TINY_BLOCK_COUNT > 0U */		/*## attribute tinyBlockMax */
};

/***    User implicit entries    ***/


/* Constructors and destructors:*/


/** 
 *  initializes all high water marks variables
 */ 
/*#[ requirement(s) */
/**
 *  Satisfies requirement 4079: HighWaterMarks - Init
 *  HighWaterMarks shall initialize all high water mark counters with 0.
 *  
 */
/*#]*/
/*## operation Init() */
void RXF_HighWaterMarks_Init(void);

extern struct RXF_HighWaterMarks_t RXF_HighWaterMarks;

#endif /* RXF_HIGH_WATER_MARKS_ENABLED */


#endif
