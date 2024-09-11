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


#ifndef RTOS_COORTX_RXF_Startup_H
#define RTOS_COORTX_RXF_Startup_H

#include "RXF_StandardTypes.h"
/*## package selectable::RTOS::COORTX */

/*## class TopLevel::RXF_Startup */
/*#[ ignore */
/** 
 *  RXF Class RXF_Startup
 */ 
/*#]*/

/** 
 *  Forward declaration for entry point of generated model
 */ 
extern int32_t modelMain( void);

/** 
 *  Rhapsody code generator compatible access to the RXF_Startup_Init() operation.
 */ 
/* #MISRA_D4.9=OFF ID-0007: Function-like macro required for Rhapsody compatibility. */   
#define RiCOXFInit(ARGC, ARGV, PORT, NAME, UNUSED1, UNUSED2, UNUSED3) RXF_Startup_Init()
/* #MISRA_D4.9=ON (end of ID-0007) */

/** 
 *  Define to declare RXF as OORTX. 
 *  This is define can be used inside user code and monitor.
 */ 
/* #MISRA_2.5=OFF ID-0006: Required for RXF Monitor compatibility. Unused when Monitor is deactivated. */   
#define RXF_OORTX
/* #MISRA_2.5=ON (end of ID-0006) */
/***    User explicit entries    ***/


/* Operations */


/**
 *  Initialize the RXF by calling other RXF class initializers in the defined order.
 *  Using full name including file name as normal operation instead of initializer to keep Rhapsody from generating the  
 *  class struct with emptyStructDummyAttribute.
 */ 
/**
 *  Satisfies requirement 4098: Startup - Init
 *  Startup shall provide an operation to initialize the RXF with all its parts.
 *  
 */
bool RXF_Startup_Init(void);


/** 
 *  Cause there is no main task, there is no entry point needed to start executing the main task.
 */ 
/*## operation RXF_Startup_start() */
/* #MISRA_D4.9=OFF ID-0102: empty Function-like macro required because the RXF is not started to process events. It is called periodically. */   
#define RXF_Startup_start()
/* #MISRA_D4.9=ON (end of ID-0102) */

#endif
