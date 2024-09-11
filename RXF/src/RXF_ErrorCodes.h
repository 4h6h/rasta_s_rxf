/*
 * RXF_ErrorCodes 
 *
 * This file belongs to the Willert Software Tools RXF Product  
 * RXF_Rpy_C.
 *
 * Copyright (c) 2023, Willert Software Tools GmbH.
 *
 * E-mail: support(at)willert.de
 */


#ifndef generic_RXF_ErrorCodes_H
#define generic_RXF_ErrorCodes_H

#include "RXF_StandardTypes.h"
/*## package generic */

/*## class TopLevel::RXF_ErrorCodes */
/*#[ ignore */
/** 
 *  RXF Class RXF_ErrorCodes
 */ 
/*#]*/

/** 
 *  Enumeration of error codes that may occur in the RXF.
 */ 
typedef enum 
{
    RXF_ERROR_EMPTY_ME_PTR = 1, /* An RXF operation was called with me == NULL. */
    RXF_ERROR_TASK_INIT_FAILED = 10, /* Could not initialize an RTOS task. Check task configuration. */
    RXF_ERROR_TASK_START_FAILED = 11, /* Could not start an RTOS task. Check task configuration. */
    RXF_ERROR_TASK_DELETE_FAILED = 12, /* Could not delete an RTOS task. */
    RXF_ERROR_TASK_SUSPEND_FAILED = 13, /* Could not suspend a task. */
    RXF_ERROR_TASK_RESUME_FAILED = 14, /* Could not resume a task. */
    RXF_ERROR_TIMER_CREATE_FAILED = 20, /* Could not create a timeout. */
    RXF_ERROR_TIMER_START_FAILED = 21, /* Could not start a timeout. */
    RXF_ERROR_MUTEX_CREATE_FAILED = 30, /* Could not create an RTOS mutex. */
    RXF_ERROR_MUTEX_CLEANUP_FAILED = 31, /* Could not cleanup an RTOS mutex. */
    RXF_ERROR_MUTEX_LOCK_FAILED = 32, /* Could not lock mutex. */
    RXF_ERROR_MUTEX_UNLOCK_FAILED = 33, /* Could not unlock mutex. */
    RXF_ERROR_MEMORY_ALLOC_TOO_LARGE = 40, /* The requested size is larger than the largest configured static memory block. */
    RXF_ERROR_MEMORY_CONFIG_ERROR = 41, /* RXF_MemoryManager_getMemory() does not have enough free memory to return the desired size. Check block sizes and counts. */
    RXF_ERROR_MEMORY_INVALID_RETURN_PTR = 42, /* The pointer passed to returnMemory() is invalid. It is some other pointer, not retrieved using getMemory(). */
    RXF_ERROR_REACTIVE_EVENT_NOT_CONSUMED = 50, /* A statemachine could not consume an event and no handleEventNotConsumed operation was set. */
    RXF_ERROR_REACTIVE_EVENT_REUSE_DETECTED = 51, /* An event was re-used, i.e. the same event object is sent again while it is still queued. */
    RXF_ERROR_REACTIVE_EMPTY_VTBL_PTR = 52, /* A virtual table is used to call the generated statechart functions. It may never be NULL. */
    RXF_ERROR_REACTIVE_VTBL_EMPTY_DISPATCHEVENT_PTR = 53, /* RXF_Reactive handles a statechart but the vtbl contains a NULL pointer to the event handling function. */
    RXF_ERROR_TOO_FEW_NULL_CONFIGS = 54, /* The null transition count has in invalid value. Check for application code generation problems. */
    RXF_ERROR_INFINITE_NULL_TRANSITIONS = 55, /* The configured null transition count has exceeded. */
    RXF_ERROR_REACTIVE_EMPTY_TASK_PTR = 56, /* No context is specified for a reactive object */
    RXF_ERROR_REACTIVE_EMPTY_EVENT_PTR = 57, /* No event is specified for a reactive object */
    RXF_ERROR_REACTIVE_RESTART_DETECTED = 58, /* A reactive instance is already started. */
    RXF_ERROR_REACTIVE_RECURSIVE_TRIGGERED_OPERATION_CALL = 59, /* Detected recursive triggered operation call. */
    RXF_ERROR_EVENT_DESTINATION_NULL = 60, /* Attempt to send an event to a NULL destination. */
    RXF_ERROR_RTOS_INIT_FAILED = 80 /* RTOS initialization failed. Check RTOS and main task configurations. */
} ErrorCode;

#endif
