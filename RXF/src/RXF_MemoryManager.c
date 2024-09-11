/*
 * RXF_MemoryManager 
 *
 * This file belongs to the Willert Software Tools RXF Product  
 * RXF_Rpy_C.
 *
 * Copyright (c) 2023, Willert Software Tools GmbH.
 *
 * E-mail: support(at)willert.de
 */


#include "RXF_MemoryManager.h"
#include "RXF_Config.h"
#include "RXF_CriticalSection.h"
#include "RXF_ErrorHandler.h"
#include "RXF_HighWaterMarks.h"
#include <stdlib.h>
/*## package generic */

/*## class TopLevel::RXF_MemoryManager */

/** 
 *  Structure holding the memory for a single pre-allocated block and a pointer to the next tiny block.
 */ 
#if (RXF_MEMORY_TINY_BLOCK_COUNT > 0U)
typedef struct TinyBlock_t
{
    
    /** 
     *  Array of pointers to allocate memory for a tiny memory block.
     */ 
    struct TinyBlock_t *blockMemory[(RXF_MEMORY_TINY_BLOCK_SIZE+(sizeof(void*)-1U))/sizeof(void*)];		/*## attribute blockMemory */
} TinyBlock_t;
#endif /* RXF_MEMORY_TINY_BLOCK_COUNT > 0U */


/** 
 *  Structure holding the memory for a single pre-allocated block and a pointer to the next small block.
 */ 
#if (RXF_MEMORY_SMALL_BLOCK_COUNT > 0U)
typedef struct SmallBlock_t
{
    
    /** 
     *  Array of pointers to allocate memory for a small memory block.
     */ 
    struct SmallBlock_t *blockMemory[(RXF_MEMORY_SMALL_BLOCK_SIZE+(sizeof(void*)-1U))/sizeof(void*)];		/*## attribute blockMemory */
} SmallBlock_t;
#endif /* RXF_MEMORY_SMALL_BLOCK_COUNT > 0U */


/** 
 *  Structure holding the memory for a single pre-allocated block and a pointer to the next medium block.
 */ 
#if (RXF_MEMORY_MEDIUM_BLOCK_COUNT > 0U)
typedef struct MediumBlock_t
{
    
    /** 
     *  Array of pointers to allocate memory for a medium memory block.
     */ 
    struct MediumBlock_t *blockMemory[(RXF_MEMORY_MEDIUM_BLOCK_SIZE+(sizeof(void*)-1U))/sizeof(void*)];		/*## attribute blockMemory */
} MediumBlock_t;
#endif /* RXF_MEMORY_MEDIUM_BLOCK_COUNT > 0U */


/** 
 *  Structure holding the memory for a single pre-allocated block and a pointer to the next large block.
 */ 
#if (RXF_MEMORY_LARGE_BLOCK_COUNT > 0U)
typedef struct LargeBlock_t
{
    
    /** 
     *  Array of pointers to allocate memory for a large memory block.
     */ 
    struct LargeBlock_t *blockMemory[(RXF_MEMORY_LARGE_BLOCK_SIZE+(sizeof(void*)-1U))/sizeof(void*)];		/*## attribute blockMemory */
} LargeBlock_t;
#endif /* RXF_MEMORY_LARGE_BLOCK_COUNT > 0U */


/** 
 *  Structure holding the memory for a single pre-allocated block and a pointer to the next huge block.
 */ 
#if (RXF_MEMORY_HUGE_BLOCK_COUNT > 0U)
typedef struct HugeBlock_t
{
    
    /** 
     *  Array of pointers to allocate memory for a huge memory block.
     */ 
    struct HugeBlock_t *blockMemory[(RXF_MEMORY_HUGE_BLOCK_SIZE+(sizeof(void*)-1U))/sizeof(void*)];		/*## attribute blockMemory */
} HugeBlock_t;
#endif /* RXF_MEMORY_HUGE_BLOCK_COUNT > 0U */


/** 
 *  Tiny memory pool, an array of tiny memory blocks. Size and number of elements are configured via RXF_Config.h.
 */ 
/*## attribute tinyPoolMemory */
#if (RXF_MEMORY_TINY_BLOCK_COUNT > 0U)
static TinyBlock_t tinyPoolMemory[RXF_MEMORY_TINY_BLOCK_COUNT];
#endif /* RXF_MEMORY_TINY_BLOCK_COUNT > 0U */


/** 
 *  Reference to the first available tiny memory block.
 */ 
/*## attribute firstTinyBlock */
#if (RXF_MEMORY_TINY_BLOCK_COUNT > 0U)
static TinyBlock_t * firstTinyBlock;
#endif /* RXF_MEMORY_TINY_BLOCK_COUNT > 0U */


/** 
 *  Small memory pool, an array of small memory blocks. Size and number of elements are configured via RXF_Config.h.
 */ 
/*## attribute smallPoolMemory */
#if (RXF_MEMORY_SMALL_BLOCK_COUNT > 0U)
static SmallBlock_t smallPoolMemory[RXF_MEMORY_SMALL_BLOCK_COUNT];
#endif /* RXF_MEMORY_SMALL_BLOCK_COUNT > 0U */


/** 
 *  Reference to the first available small memory block.
 */ 
/*## attribute firstSmallBlock */
#if (RXF_MEMORY_SMALL_BLOCK_COUNT > 0U)
static SmallBlock_t * firstSmallBlock;
#endif /* RXF_MEMORY_SMALL_BLOCK_COUNT > 0U */


/** 
 *  Medium memory pool, an array of medium memory blocks. Size and number of elements are configured via RXF_Config.h.
 */ 
/*## attribute mediumPoolMemory */
#if (RXF_MEMORY_MEDIUM_BLOCK_COUNT > 0U)
static MediumBlock_t mediumPoolMemory[RXF_MEMORY_MEDIUM_BLOCK_COUNT];
#endif /* RXF_MEMORY_MEDIUM_BLOCK_COUNT > 0U */


/** 
 *  Reference to the first available medium memory block.
 */ 
/*## attribute firstMediumBlock */
#if (RXF_MEMORY_MEDIUM_BLOCK_COUNT > 0U)
static MediumBlock_t * firstMediumBlock;
#endif /* RXF_MEMORY_MEDIUM_BLOCK_COUNT > 0U */


/** 
 *  Large memory pool, an array of large memory blocks. Size and number of elements are configured via RXF_Config.h.
 */ 
/*## attribute largePoolMemory */
#if (RXF_MEMORY_LARGE_BLOCK_COUNT > 0U)
static LargeBlock_t largePoolMemory[RXF_MEMORY_LARGE_BLOCK_COUNT];
#endif /* RXF_MEMORY_LARGE_BLOCK_COUNT > 0U */


/** 
 *  Reference to the first available large memory block.
 */ 
/*## attribute firstLargeBlock */
#if (RXF_MEMORY_LARGE_BLOCK_COUNT > 0U)
static LargeBlock_t * firstLargeBlock;
#endif /* RXF_MEMORY_LARGE_BLOCK_COUNT > 0U */


/** 
 *  Huge memory pool, an array of huge memory blocks. Size and number of elements are configured via RXF_Config.h.
 */ 
/*## attribute hugePoolMemory */
#if (RXF_MEMORY_HUGE_BLOCK_COUNT > 0U)
static HugeBlock_t hugePoolMemory[RXF_MEMORY_HUGE_BLOCK_COUNT];
#endif /* RXF_MEMORY_HUGE_BLOCK_COUNT > 0U */


/** 
 *  Reference to the first available huge memory block.
 */ 
/*## attribute firstHugeBlock */
#if (RXF_MEMORY_HUGE_BLOCK_COUNT > 0U)
static HugeBlock_t * firstHugeBlock;
#endif /* RXF_MEMORY_HUGE_BLOCK_COUNT > 0U */


/** 
 *  Dynamic allocation of memory from heap.
 *
 *    requestedSize (In): Size of the required memory block in bytes.
 *  
 */ 
/*## operation dynamicAlloc(MALLOC_ARGUMENT_TYPE) */
#ifdef RXF_MEMORY_USE_HEAP
static MALLOC_RETURN_TYPE dynamicAlloc(MALLOC_ARGUMENT_TYPE requestedSize);
#endif /* RXF_MEMORY_USE_HEAP */     



/** 
 *  Return memory to heap.
 *
 *    blockMemory (In): Pointer to the memory block that will be returned to the pool.
 *  
 */ 
/*## operation dynamicReturn(FREE_ARGUMENT_TYPE) */
#ifdef RXF_MEMORY_USE_HEAP
static void dynamicReturn(FREE_ARGUMENT_TYPE blockMemory);
#endif /* RXF_MEMORY_USE_HEAP */     


/*## operation Init() */
void RXF_MemoryManager_Init(void)
{
    uint16_t i;
    
    #if (RXF_MEMORY_TINY_BLOCK_COUNT > 0U)
    firstTinyBlock = &tinyPoolMemory[0];
    for(i = 0U; i < (RXF_MEMORY_TINY_BLOCK_COUNT - 1U); i++)
    {
    	tinyPoolMemory[ i ].blockMemory[0] = &tinyPoolMemory[ i + 1U ];
    }
    tinyPoolMemory[ RXF_MEMORY_TINY_BLOCK_COUNT - 1U ].blockMemory[0] = NULL;
    #endif /* RXF_MEMORY_TINY_BLOCK_COUNT > 0U */
    
    #if (RXF_MEMORY_SMALL_BLOCK_COUNT > 0U)
    firstSmallBlock = &smallPoolMemory[0];
    for(i = 0U; i < (RXF_MEMORY_SMALL_BLOCK_COUNT - 1U); i++)
    {
    	smallPoolMemory[ i ].blockMemory[0] = &smallPoolMemory[ i + 1U ];
    }
    smallPoolMemory[ RXF_MEMORY_SMALL_BLOCK_COUNT - 1U ].blockMemory[0] = NULL;
    #endif /* RXF_MEMORY_SMALL_BLOCK_COUNT > 0U */
    
    #if (RXF_MEMORY_MEDIUM_BLOCK_COUNT > 0U)
    firstMediumBlock = &mediumPoolMemory[0];
    for(i = 0U; i < (RXF_MEMORY_MEDIUM_BLOCK_COUNT - 1U); i++)
    {
    	mediumPoolMemory[ i ].blockMemory[0] = &mediumPoolMemory[ i + 1U ];
    }
    mediumPoolMemory[ RXF_MEMORY_MEDIUM_BLOCK_COUNT - 1U ].blockMemory[0] = NULL;
    #endif /* RXF_MEMORY_MEDIUM_BLOCK_COUNT > 0U */
    
    #if (RXF_MEMORY_LARGE_BLOCK_COUNT > 0U)
    firstLargeBlock = &largePoolMemory[0];
    for(i = 0U; i < (RXF_MEMORY_LARGE_BLOCK_COUNT - 1U); i++)
    {
    	largePoolMemory[ i ].blockMemory[0] = &largePoolMemory[ i + 1U ];
    }
    largePoolMemory[ RXF_MEMORY_LARGE_BLOCK_COUNT - 1U ].blockMemory[0] = NULL;
    #endif /* RXF_MEMORY_LARGE_BLOCK_COUNT > 0U */
    
    #if (RXF_MEMORY_HUGE_BLOCK_COUNT > 0U)
    firstHugeBlock = &hugePoolMemory[0];
    for(i = 0U; i < (RXF_MEMORY_HUGE_BLOCK_COUNT - 1U); i++)
    {
    	hugePoolMemory[ i ].blockMemory[0] = &hugePoolMemory[ i + 1U ];
    }
    hugePoolMemory[ RXF_MEMORY_HUGE_BLOCK_COUNT - 1U ].blockMemory[0] = NULL;
    #endif /* RXF_MEMORY_HUGE_BLOCK_COUNT > 0U */
}

/*## operation getMemory(MALLOC_ARGUMENT_TYPE) */
MALLOC_RETURN_TYPE RXF_MemoryManager_getMemory(MALLOC_ARGUMENT_TYPE requestedSize)
{
    MALLOC_RETURN_TYPE	p = NULL;
    
    #if (RXF_MEMORY_TINY_BLOCK_COUNT > 0U)
    if ( requestedSize <= RXF_MEMORY_TINY_BLOCK_SIZE )
    {
    	RXF_CRITICALSECTION_VARIABLE	
    	RXF_CRITICALSECTION_LOCK_CALL
    
    	p = (MALLOC_RETURN_TYPE) firstTinyBlock;
    	
    	if( NULL != firstTinyBlock )
    	{
    		firstTinyBlock = firstTinyBlock->blockMemory[ 0 ];
    	}
    
    	RXF_CRITICALSECTION_UNLOCK_CALL
    
    #ifdef RXF_HIGH_WATER_MARKS_ENABLED
    
    	RXF_HighWaterMarks.tinyBlockCount++;
    
    	if( RXF_HighWaterMarks.tinyBlockCount > RXF_HighWaterMarks.tinyBlockMax )
    	{
    		RXF_HighWaterMarks.tinyBlockMax = RXF_HighWaterMarks.tinyBlockCount;
    	}
    
    #endif /* RXF_HIGH_WATER_MARKS_ENABLED */
    
    }
    else
    #endif /* RXF_MEMORY_TINY_BLOCK_COUNT > 0U */
    
    #if (RXF_MEMORY_SMALL_BLOCK_COUNT > 0U)
    if ( requestedSize <= RXF_MEMORY_SMALL_BLOCK_SIZE )
    {
    	RXF_CRITICALSECTION_VARIABLE
    	RXF_CRITICALSECTION_LOCK_CALL
    
    	p = (MALLOC_RETURN_TYPE) firstSmallBlock;
    
    	if( NULL != firstSmallBlock )
    	{
    		firstSmallBlock = firstSmallBlock->blockMemory[0];
    	}
    
    	RXF_CRITICALSECTION_UNLOCK_CALL
    
    #ifdef RXF_HIGH_WATER_MARKS_ENABLED
    
    	RXF_HighWaterMarks.smallBlockCount++;
    
    	if( RXF_HighWaterMarks.smallBlockCount > RXF_HighWaterMarks.smallBlockMax )
    	{
    		RXF_HighWaterMarks.smallBlockMax = RXF_HighWaterMarks.smallBlockCount;
    	}
    
    #endif /* RXF_HIGH_WATER_MARKS_ENABLED */
    
    }
    else
    #endif /* RXF_MEMORY_SMALL_BLOCK_COUNT > 0U */
    
    #if (RXF_MEMORY_MEDIUM_BLOCK_COUNT > 0U)
    if ( requestedSize <= RXF_MEMORY_MEDIUM_BLOCK_SIZE )
    {
    	
    	RXF_CRITICALSECTION_VARIABLE
    	RXF_CRITICALSECTION_LOCK_CALL
    
    	p = (MALLOC_RETURN_TYPE) firstMediumBlock;
    
    	if( NULL != firstMediumBlock )
    	{
    		firstMediumBlock = firstMediumBlock->blockMemory[0];
    	}
    
    	RXF_CRITICALSECTION_UNLOCK_CALL
    
    #ifdef RXF_HIGH_WATER_MARKS_ENABLED
    
       		RXF_HighWaterMarks.mediumBlockCount++;
    
    		if( RXF_HighWaterMarks.mediumBlockCount > RXF_HighWaterMarks.mediumBlockMax )
    		{
    			RXF_HighWaterMarks.mediumBlockMax = RXF_HighWaterMarks.mediumBlockCount;
    		}
    
    #endif /* RXF_HIGH_WATER_MARKS_ENABLED */
    
    }
    else
    #endif /* RXF_MEMORY_MEDIUM_BLOCK_COUNT > 0U */
    
    #if (RXF_MEMORY_LARGE_BLOCK_COUNT > 0U)
    if ( requestedSize <= RXF_MEMORY_LARGE_BLOCK_SIZE )
    {
    
    	RXF_CRITICALSECTION_VARIABLE
    	RXF_CRITICALSECTION_LOCK_CALL
    
    	p = (MALLOC_RETURN_TYPE) firstLargeBlock;
    	
    	if( NULL != firstLargeBlock )
    	{
    		firstLargeBlock = firstLargeBlock->blockMemory[0];
    	}		
    
    	RXF_CRITICALSECTION_UNLOCK_CALL
    
    #ifdef RXF_HIGH_WATER_MARKS_ENABLED
    
    	RXF_HighWaterMarks.largeBlockCount++;
    
    	if( RXF_HighWaterMarks.largeBlockCount > RXF_HighWaterMarks.largeBlockMax )
    	{
    		RXF_HighWaterMarks.largeBlockMax = RXF_HighWaterMarks.largeBlockCount;
    	}
    
    #endif /* RXF_HIGH_WATER_MARKS_ENABLED */
    
    }
    else
    #endif /* RXF_MEMORY_LARGE_BLOCK_COUNT > 0U */
    
    #if (RXF_MEMORY_HUGE_BLOCK_COUNT > 0U)
    if ( requestedSize <= RXF_MEMORY_HUGE_BLOCK_SIZE )
    {
    
    	RXF_CRITICALSECTION_VARIABLE
    	RXF_CRITICALSECTION_LOCK_CALL
    
    	p = (MALLOC_RETURN_TYPE) firstHugeBlock;
    	
    	if( NULL != firstHugeBlock )
    	{
    		firstHugeBlock = firstHugeBlock->blockMemory[0];
    	}		
    
    	RXF_CRITICALSECTION_UNLOCK_CALL
    
    #ifdef RXF_HIGH_WATER_MARKS_ENABLED
    
    	RXF_HighWaterMarks.hugeBlockCount++;
    
    	if( RXF_HighWaterMarks.hugeBlockCount > RXF_HighWaterMarks.hugeBlockMax )
    	{
    		RXF_HighWaterMarks.hugeBlockMax = RXF_HighWaterMarks.hugeBlockCount;
    	}
    
    #endif /* RXF_HIGH_WATER_MARKS_ENABLED */
    
    }         
    else
    #endif /* RXF_MEMORY_HUGE_BLOCK_COUNT > 0U */
    
    {
    	;
    	/* This is an intentionally empty else block because a previous if( ...COUNT > 0 )
    	 * will expect an else statement.
    	 */
    }
    
    #if ( ( defined RXF_MEMORY_FORCE_ALLOC_TOO_LARGE ) || ( !defined RXF_MEMORY_USE_HEAP ) )
    if (requestedSize > RXF_MEMORY_MAX_BLOCK_SIZE)
    {
    	RXF_ErrorHandler_error(RXF_ERROR_MEMORY_ALLOC_TOO_LARGE, (int_t)requestedSize, true);
    }
    #endif
    
    #ifdef RXF_MEMORY_USE_HEAP
    if ( NULL == p )
    {
    	p = dynamicAlloc( requestedSize );
    }
    #endif
    
    if ( NULL == p )
    {
    	if( requestedSize <= RXF_MEMORY_MAX_BLOCK_SIZE )
    	{
    		#ifdef RXF_HIGH_WATER_MARKS_ENABLED  
    		RXF_HighWaterMarks.blockOverflow = true;   
    		#endif /* RXF_HIGH_WATER_MARKS_ENABLED */
    
    		RXF_ErrorHandler_error( RXF_ERROR_MEMORY_CONFIG_ERROR, (int_t) requestedSize, true );
    	}
    }
    
    return p;
}

/*## operation returnMemory(FREE_ARGUMENT_TYPE) */
void RXF_MemoryManager_returnMemory(FREE_ARGUMENT_TYPE blockMemory)
{
    #if (RXF_MEMORY_TINY_BLOCK_COUNT > 0U)
    /* #MISRA_18.3=OFF ID-0011: Comparison needed to identify corresponding memory pool. */
    if (  ( blockMemory >= (FREE_ARGUMENT_TYPE) &tinyPoolMemory[ 0U ] ) &&
          ( blockMemory <= (FREE_ARGUMENT_TYPE) &tinyPoolMemory[ RXF_MEMORY_TINY_BLOCK_COUNT - 1U ] ) )
    /* #MISRA_18.3=ON */
    {
    	/* #MISRA_11.3,11.5=OFF ID-0010: Pointercast from FREE_ARGUMENT_TYPE to TinyBlock_t */
    	TinyBlock_t *tmp = (TinyBlock_t*)blockMemory;
    	/* #MISRA_11.3,11.5=ON */
    	
    	RXF_CRITICALSECTION_VARIABLE
    	RXF_CRITICALSECTION_LOCK_CALL
    
    	tmp->blockMemory[ 0 ] = firstTinyBlock;
    	firstTinyBlock = tmp;
    
    	RXF_CRITICALSECTION_UNLOCK_CALL    
    	
    #ifdef RXF_HIGH_WATER_MARKS_ENABLED
    
    	RXF_HighWaterMarks.tinyBlockCount--;
    
    #endif /* RXF_HIGH_WATER_MARKS_ENABLED */
    }
    else
    #endif /* RXF_MEMORY_TINY_BLOCK_COUNT > 0U */
    
    
    #if (RXF_MEMORY_SMALL_BLOCK_COUNT > 0U)
    /* #MISRA_18.3=OFF ID-0011: Comparison needed to identify corresponding memory pool. */
    if (  ( blockMemory >= (FREE_ARGUMENT_TYPE) &smallPoolMemory[ 0U ] ) &&
          ( blockMemory <= (FREE_ARGUMENT_TYPE) &smallPoolMemory[ RXF_MEMORY_SMALL_BLOCK_COUNT - 1U ] ) )
    /* #MISRA_18.3=ON */
    {
    	/* #MISRA_11.3,11.5=OFF ID-0010: Pointercast from FREE_ARGUMENT_TYPE to SmallBlock_t */
    	SmallBlock_t *tmp = (SmallBlock_t*)blockMemory;
    	/* #MISRA_11.3,11.5=ON */
    	
    	RXF_CRITICALSECTION_VARIABLE	
    	RXF_CRITICALSECTION_LOCK_CALL
    
    	tmp->blockMemory[0] = firstSmallBlock;
    	firstSmallBlock = tmp;
    
    	RXF_CRITICALSECTION_UNLOCK_CALL
    	
    #ifdef RXF_HIGH_WATER_MARKS_ENABLED
    
    	RXF_HighWaterMarks.smallBlockCount--;
    
    #endif /* RXF_HIGH_WATER_MARKS_ENABLED */
    }
    else
    #endif /* RXF_MEMORY_SMALL_BLOCK_COUNT > 0U */
    
    #if (RXF_MEMORY_MEDIUM_BLOCK_COUNT > 0U)
    /* #MISRA_18.3=OFF ID-0011: Comparison needed to identify corresponding memory pool. */
    if ( ( blockMemory >= (FREE_ARGUMENT_TYPE) &mediumPoolMemory[ 0U ] ) &&
    	 ( blockMemory <= (FREE_ARGUMENT_TYPE) &mediumPoolMemory[ RXF_MEMORY_MEDIUM_BLOCK_COUNT - 1U ] ) )
    /* #MISRA_18.3=ON */
    {
    	/* #MISRA_11.3,11.5=OFF ID-0010: Pointercast from FREE_ARGUMENT_TYPE to MediumBlock_t */
    	MediumBlock_t *tmp = (MediumBlock_t*)blockMemory;
    	/* #MISRA_11.3,11.5=ON */
    	
    	RXF_CRITICALSECTION_VARIABLE
    	RXF_CRITICALSECTION_LOCK_CALL
    
    	tmp->blockMemory[0] = firstMediumBlock;
    	firstMediumBlock = tmp;
    
    	RXF_CRITICALSECTION_UNLOCK_CALL
    
    #ifdef RXF_HIGH_WATER_MARKS_ENABLED
    
    	RXF_HighWaterMarks.mediumBlockCount--;
    
    #endif /* RXF_HIGH_WATER_MARKS_ENABLED */
    }
    else
    #endif /* RXF_MEMORY_MEDIUM_BLOCK_COUNT > 0U */
    
    #if (RXF_MEMORY_LARGE_BLOCK_COUNT > 0U)
    /* #MISRA_18.3=OFF ID-0011: Comparison needed to identify corresponding memory pool. */
    if ( ( blockMemory >= (FREE_ARGUMENT_TYPE) &largePoolMemory[ 0U ] ) &&
    	 ( blockMemory <= (FREE_ARGUMENT_TYPE) &largePoolMemory[ RXF_MEMORY_LARGE_BLOCK_COUNT - 1U ] ) )
    /* #MISRA_18.3=ON */
    {
    	/* #MISRA_11.3,11.5=OFF ID-0010: Pointercast from FREE_ARGUMENT_TYPE to LargeBlock_t */
    	LargeBlock_t *tmp = (LargeBlock_t*)blockMemory;
    	/* #MISRA_11.3,11.5=ON */
    	
    	RXF_CRITICALSECTION_VARIABLE
    	RXF_CRITICALSECTION_LOCK_CALL
    
    	tmp->blockMemory[0] = firstLargeBlock;
    	firstLargeBlock = tmp;
    
    	RXF_CRITICALSECTION_UNLOCK_CALL
    
    #ifdef RXF_HIGH_WATER_MARKS_ENABLED
    
    	RXF_HighWaterMarks.largeBlockCount--;
    
    #endif /* RXF_HIGH_WATER_MARKS_ENABLED */
    }
    else
    #endif /* RXF_MEMORY_LARGE_BLOCK_COUNT > 0U */
    
    #if (RXF_MEMORY_HUGE_BLOCK_COUNT > 0U)
    /* #MISRA_18.3=OFF ID-0011: Comparison needed to identify corresponding memory pool. */
    if ( ( blockMemory >= (FREE_ARGUMENT_TYPE) &hugePoolMemory[ 0U ] ) &&
    	 ( blockMemory <= (FREE_ARGUMENT_TYPE) &hugePoolMemory[ RXF_MEMORY_HUGE_BLOCK_COUNT - 1U ] ) )
    /* #MISRA_18.3=ON */
    {
    	/* #MISRA_11.3,11.5=OFF ID-0010: Pointercast from FREE_ARGUMENT_TYPE to HugeBlock_t */
    	HugeBlock_t *tmp = (HugeBlock_t*)blockMemory;
    	/* #MISRA_11.3,11.5=ON */
    	
    	
    	RXF_CRITICALSECTION_VARIABLE
    	RXF_CRITICALSECTION_LOCK_CALL
    
    	tmp->blockMemory[0] = firstHugeBlock;
    	firstHugeBlock = tmp;
    
    	RXF_CRITICALSECTION_UNLOCK_CALL
    
    #ifdef RXF_HIGH_WATER_MARKS_ENABLED
    
    	RXF_HighWaterMarks.hugeBlockCount--;
    
    #endif /* RXF_HIGH_WATER_MARKS_ENABLED */
    }
    else
    #endif /* RXF_MEMORY_HUGE_BLOCK_COUNT > 0U */
    {
    #ifdef RXF_MEMORY_USE_HEAP
    	dynamicReturn( blockMemory );
    #else /* RXF_MEMORY_USE_HEAP */
    	RXF_ErrorHandler_error( RXF_ERROR_MEMORY_INVALID_RETURN_PTR, 0, true  );
    #endif /* RXF_MEMORY_USE_HEAP */
    }
    
}

/*## operation dynamicAlloc(MALLOC_ARGUMENT_TYPE) */
#ifdef RXF_MEMORY_USE_HEAP
static MALLOC_RETURN_TYPE dynamicAlloc(MALLOC_ARGUMENT_TYPE requestedSize)
{
    MALLOC_RETURN_TYPE	p = NULL;
    RXF_CRITICALSECTION_VARIABLE
    
    RXF_CRITICALSECTION_LOCK_CALL
    p = malloc( requestedSize );
    RXF_CRITICALSECTION_UNLOCK_CALL
    
    return p;
}
#endif /* RXF_MEMORY_USE_HEAP */ 


/*## operation dynamicReturn(FREE_ARGUMENT_TYPE) */
#ifdef RXF_MEMORY_USE_HEAP
static void dynamicReturn(FREE_ARGUMENT_TYPE blockMemory)
{
    RXF_CRITICALSECTION_VARIABLE
    
    RXF_CRITICALSECTION_LOCK_CALL
    free( blockMemory );
    RXF_CRITICALSECTION_UNLOCK_CALL
}
#endif /* RXF_MEMORY_USE_HEAP */ 


