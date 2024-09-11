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


#ifndef generic_RXF_MemoryManager_H
#define generic_RXF_MemoryManager_H

#include "RXF_StandardTypes.h"
/*## package generic */

/*## class TopLevel::RXF_MemoryManager */
/*#[ ignore */
/** 
 *  RXF Class RXF_MemoryManager
 *
 *  Memory management class of the RXF. Used for pseudo-dynamic memory allocation from statically allocated memory blocks of  
 *  fixed sizes, organized in memory pools containing the blocks.
 *  It is implemented via static inheritance in the selectable package.
 */ 
/*#]*/

/** 
 *  Type definition for the memory allocation size argument.
 */ 
typedef size_t MALLOC_ARGUMENT_TYPE;

/** 
 *  Type definition for the memory allocation return type.
 */ 
typedef void* MALLOC_RETURN_TYPE;


/** 
 *  Type definition for the memory return argument.
 */ 
typedef void* FREE_ARGUMENT_TYPE;

/***    User explicit entries    ***/


/* Operations */


/** 
 *  Initializes the pointers of the three static memory pools.
 *  Using full name including file name as normal operation instead of initializer to keep Rhapsody from generating the  
 *  class struct with emptyStructDummyAttribute.
 */ 
/*#[ requirement(s) */
/**
 *  Satisfies requirement 4091: MemoryManager - Disable Pools
 *  When the block count of a memory pool equals 0, the MemoryManager shall switch off this pool.
 *  
 *  Satisfies requirement 4076: MemoryManager - Memory Pools
 *  MemoryManager shall own up to five different memory pools.
 *  
 *  Satisfies requirement 129: MemoryManager - Initialisation
 *  MemoryManager shall initialize its memory pools with different block sizes and block counts.
 *  
 */
/*#]*/
/*## operation Init() */
void RXF_MemoryManager_Init(void);


/** 
 *  Allocation of memory from pre-allocated memory pools (see attributes). If all (large enough) memory blocks are already  
 *  in use, RXF will decide depending on the define RXF_MEMORY_USE_HEAP to call the error handler or use heap memory instead.
 *
 *    requestedSize (In): Size of the required memory block in bytes.
 *  
 */ 
/*#[ requirement(s) */
/**
 *  Satisfies requirement 4091: MemoryManager - Disable Pools
 *  When the block count of a memory pool equals 0, the MemoryManager shall switch off this pool.
 *  
 *  Satisfies requirement 4080: MemoryManager - Error no Free Block
 *  MemoryManager shall call the ErrorHandler, if memory is requested and no more free blocks are available from the associated memory pool.
 *  
 *  Satisfies requirement 4077: MemoryManager - Error too Large
 *  MemoryManager shall call the ErrorHandler when the requested size is larger than the block size of the largest pool.
 *  
 *  Satisfies requirement 2840: MemoryManager - Track HighWaterMarks
 *  MemoryManager shall track the maximum count of parallel used blocks for debugging purposes in HighWaterMarks.
 *  
 *  Satisfies requirement 164: MemoryManager - No Blocks Available
 *  MemoryManager shall return NULL, if memory is requested and no more free blocks are available from the associated memory pool.
 *  
 *  Satisfies requirement 167: MemoryManager - Allocate by Size
 *  MemoryManager shall provide valid memory, if the requested size is smaller or equal than the defined block size of the largest memory pool.
 *  
 *  Satisfies requirement 2839: MemoryManager - Invalid Memory Allocation
 *  MemoryManager shall return NULL, if the requested size is larger than the block size of the largest pool.
 *  
 *  Satisfies requirement 4076: MemoryManager - Memory Pools
 *  MemoryManager shall own up to five different memory pools.
 *  
 */
/*#]*/
/*## operation getMemory(MALLOC_ARGUMENT_TYPE) */
MALLOC_RETURN_TYPE RXF_MemoryManager_getMemory(MALLOC_ARGUMENT_TYPE requestedSize);


/** 
 *  Return memory to pre-allocated memory pools.
 *  If the define RXF_MEMORY_USE_HEAP is set, it may also be used to return memory from the heap.
 *
 *    blockMemory (In): Pointer to the memory block that will be returned to the pool.
 *  
 */ 
/*#[ requirement(s) */
/**
 *  Satisfies requirement 4091: MemoryManager - Disable Pools
 *  When the block count of a memory pool equals 0, the MemoryManager shall switch off this pool.
 *  
 *  Satisfies requirement 4081: MemoryManager - Error Invalid Returned Block
 *  MemoryManager shall call the ErrorHandler, if a returned memory block can not be associated to a memory pool.
 *  
 *  Satisfies requirement 2840: MemoryManager - Track HighWaterMarks
 *  MemoryManager shall track the maximum count of parallel used blocks for debugging purposes in HighWaterMarks.
 *  
 *  Satisfies requirement 2841: MemoryManager - Release Block
 *  MemoryManager shall be able to take back a previously allocated block from the associated memory pool.
 *  
 *  Satisfies requirement 4076: MemoryManager - Memory Pools
 *  MemoryManager shall own up to five different memory pools.
 *  
 */
/*#]*/
/*## operation returnMemory(FREE_ARGUMENT_TYPE) */
void RXF_MemoryManager_returnMemory(FREE_ARGUMENT_TYPE blockMemory);

#endif
