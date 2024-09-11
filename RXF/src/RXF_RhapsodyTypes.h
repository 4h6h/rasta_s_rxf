/*
 * RXF_RhapsodyTypes 
 *
 * This file belongs to the Willert Software Tools RXF Product  
 * RXF_Rpy_C.
 *
 * Copyright (c) 2023, Willert Software Tools GmbH.
 *
 * E-mail: support(at)willert.de
 */


#ifndef generic_RXF_RhapsodyTypes_H
#define generic_RXF_RhapsodyTypes_H

#include "RXF_StandardTypes.h"
/*## package generic */

/*## class TopLevel::RXF_RhapsodyTypes */
/*#[ ignore */
/** 
 *  RXF Class RXF_RhapsodyTypes
 *
 *  Rhapsody specific typedefs and defines, required for Rhapsody code generation compatibility with the RXF.
 */ 
/*#]*/

/** 
 *  Rhapsody in C specific boolean type, mapped to C99 "bool" type.
 */ 
typedef bool RiCBoolean;


/** 
 *  Rhapsody's language independent boolean type, mapped to C99 "bool" type.
 */ 
typedef bool RhpBoolean;


/** 
 *  Rhapsody specific boolean value "false".
 */ 
#define RiCFALSE (false)

/** 
 *  Rhapsody specific boolean value "true".
 */ 
#define RiCTRUE (true)

/** 
 *  Macro can be used to reply a return value for triggered operations
 */ 
/* #MISRA_D4.9=OFF ID-0007: Function-like macro required for Rhapsody compatibility. */   
#define RiCREPLY(retVal)	params->ric_reply = (retVal)
/* #MISRA_D4.9=ON (end of ID-0007) */

#endif
