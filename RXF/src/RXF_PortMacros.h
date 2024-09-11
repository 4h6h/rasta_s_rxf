/*
 * RXF_PortMacros 
 *
 * This file belongs to the Willert Software Tools RXF Product  
 * RXF_Rpy_C.
 *
 * Copyright (c) 2023, Willert Software Tools GmbH.
 *
 * E-mail: support(at)willert.de
 */


#ifndef generic_RXF_PortMacros_H
#define generic_RXF_PortMacros_H

#include "RXF_StandardTypes.h"
/*## package generic */

/*## class TopLevel::RXF_PortMacros */
/*#[ ignore */
/** 
 *  RXF Class RXF_PortMacros
 *
 *  Header file containing the event generation macros.
 */ 
/*#]*/

/** 
 *  Rhapsody specific (and oxf compatible) event generation macro. A dynamic event will be created from the memory pools  
 *  and send to the specified destination.
 */ 
#define OUT_PORT OPORT

/** 
 *  Rhapsody specific (and oxf compatible) event generation macro. A dynamic event will be created from the memory pools  
 *  and send to the specified destination.
 */ 
/* #MISRA_D4.9=OFF ID-0007: Function-like macro required for Rhapsody compatibility. */   
#define OPORT(CLASS,PORT,INTERFACE) GET_OPORT(PORT,INTERFACE)
/* #MISRA_D4.9=ON (end of ID-0007) */

/** 
 *  Rhapsody specific (and oxf compatible) event generation macro. A dynamic event will be created from the memory pools  
 *  and send to the specified destination via a port.
 */ 
/* #MISRA_D4.9=OFF ID-0007: Function-like macro required for Rhapsody compatibility. */   
#define RiCGEN_PORT_I(CLASS,PORT,INTERFACE,EVENT) CGEN_PORT_I(PORT,INTERFACE,EVENT)
/* #MISRA_D4.9=ON (end of ID-0007) */

/** 
 *  Rhapsody specific (and oxf compatible) event generation macro. A dynamic event will be created from the memory pools  
 *  and send to the specified destination via a port.
 */ 
/* #MISRA_D4.9=OFF ID-0007: Function-like macro required for Rhapsody compatibility. */   
#define RiCGEN_PORT_ISR_I(CLASS,PORT,INTERFACE,EVENT) CGEN_PORT_ISR_I(PORT,INTERFACE,EVENT)
/* #MISRA_D4.9=ON (end of ID-0007) */

/** 
 *  Rhapsody specific (and oxf compatible) event generation macro. A dynamic event will be created from the memory pools  
 *  and send to the specified destination via a port.
 */ 
#define RiCGEN_PORT CGEN_PORT

/** 
 *  Rhapsody specific (and oxf compatible) event generation macro. A dynamic event will be created from the memory pools  
 *  and send to the specified destination via a port.
 */ 
#define RiCGEN_PORT_ISR CGEN_PORT_ISR

/** 
 *  Needed for flow ports
 */ 
/* #MISRA_D4.9,20.10=OFF ID-0008: Function-like macro using concatenation operator required for Rhapsody compatibility. */   
#define RIC_CONCAT_5(A, B, C, D, E) A ##B ##C ##D ##E
/* #MISRA_D4.9,20.10=ON (end of ID-0008) */

/** 
 *  Needed for sending data via a flow port
 */ 
/* #MISRA_D4.9=OFF ID-0007: Function-like macro required for Rhapsody compatibility. */   
#define FLOW_DATA_SEND(me, flowPort, flowPortVal, servicePortName, fpType, CLASS, serializeFunc) \
	RIC_CONCAT_5(CLASS,_,servicePortName,_C_SetValue_,fpType)((me)->servicePortName, flowPortVal, NULL);
/* #MISRA_D4.9=ON (end of ID-0007) */

/** 
 *  Needed for flow ports
 */ 
/* #MISRA_D4.9=OFF ID-0007: Function-like macro required for Rhapsody compatibility. */   
#define FLOW_DATA_RECEIVE(me, argName, argValue, CLASS, serializeFunc)
/* #MISRA_D4.9=ON (end of ID-0007) */
/***    User explicit entries    ***/


/* Operations */


/** 
 *  Event generation macro. A dynamic event will be created from the memory pools and send to the specified  
 *  destination via a rapid port.
 *
 *    PORT (In): Name of the port
 *  
 *    EVENT (In): Event name followed by "(<with optional arguments>)"
 *  
 */ 
/*## operation CGEN_PORT(MacroArg_t,MacroArg_t) */
/* #MISRA_D4.9,20.10=OFF ID-0008: Function-like macro using concatenation operator required for Rhapsody compatibility. */   
#define CGEN_PORT(PORT, EVENT)    \
    {\
    	RXF_Reactive* destination = (PORT).outBound;\
    	if( destination != NULL )\
    	{\
    		RXF_Event* event = (RXF_Event*)RiC_Create_##EVENT;\
    		RXF_Reactive_gen( destination, event RXF_MONITOR_GEN_SOURCE_ARGUMENT );\
    	}\
    }
/* #MISRA_D4.9,20.10=ON (end of ID-0008) */


/** 
 *  Event generation macro. A dynamic event will be created from the memory pools and send to the specified  
 *  destination via an interface.
 *
 *    PORT (In): Name of the port
 *  
 *    INTERFACE (In): Name of the interface used by the port
 *  
 *    EVENT (In): Event name followed by "(<with optional arguments>)"
 *  
 */ 
/*## operation CGEN_PORT_I(MacroArg_t,MacroArg_t,MacroArg_t) */
/* #MISRA_D4.9,20.10=OFF ID-0008: Function-like macro using concatenation operator required for Rhapsody compatibility. */   
#define CGEN_PORT_I(PORT, INTERFACE, EVENT)    \
    {\
    	RXF_Reactive* destination = (me->PORT.outBound._##INTERFACE)->ric_reactive;\
    	if( destination != NULL )\
    	{\
    		RXF_Event* event = (RXF_Event*)RiC_Create_##EVENT;\
    		RXF_Reactive_gen( destination, event RXF_MONITOR_GEN_SOURCE_ARGUMENT );\
    	}\
    }
/* #MISRA_D4.9,20.10=ON (end of ID-0008) */


/** 
 *  Event generation macro. A static event will be created from the memory pools and send to the specified  
 *  destination via a rapid port.
 *  
 *
 *    PORT (In): Name of the port
 *  
 *    EVENT (In): Event name followed by "(<with optional arguments>)"
 *  
 */ 
/*## operation CGEN_PORT_ISR(MacroArg_t,MacroArg_t) */
/* #MISRA_D4.9=OFF ID-0007: Function-like macro required for Rhapsody compatibility. */   
#define CGEN_PORT_ISR(PORT, EVENT)    \
    {\
    	RXF_Reactive* destination = (PORT).outBound;\
    	if( destination != NULL )\
    	{\
    		RXF_Event* event = &((EVENT)->ric_event);\
    		RXF_Reactive_gen( destination, event RXF_MONITOR_GEN_SOURCE_ARGUMENT );\
    	}\
    }
/* #MISRA_D4.9=ON (end of ID-0007) */


/** 
 *  Event generation macro. A static event will be created from the memory pools and send to the specified  
 *  destination via an interface.
 *
 *    PORT (In): Name of the port
 *  
 *    INTERFACE (In): Name of the interface used by the port
 *  
 *    EVENT (In): Event name followed by "(<with optional arguments>)"
 *  
 */ 
/*## operation CGEN_PORT_ISR_I(MacroArg_t,MacroArg_t,MacroArg_t) */
/* #MISRA_D4.9,20.10=OFF ID-0008: Function-like macro using concatenation operator required for Rhapsody compatibility. */   
#define CGEN_PORT_ISR_I(PORT, INTERFACE, EVENT)    \
    {\
    	RXF_Reactive* destination = (me->PORT.outBound._##INTERFACE)->ric_reactive;\
    	if( destination != NULL )\
    	{\
    		RXF_Reactive_gen( destination, &((EVENT)->ric_event) RXF_MONITOR_GEN_SOURCE_ARGUMENT );\
    	}\
    }
/* #MISRA_D4.9,20.10=ON (end of ID-0008) */


/** 
 *  Getting outbound for a specific interface from a port
 *
 *    PORT (In): Name of the port
 *  
 *    INTERFACE (In): Name of the interface used by the port
 *  
 */ 
/*## operation GET_OPORT(MacroArg_t,MacroArg_t) */
/* #MISRA_D4.9,20.10=OFF ID-0008: Function-like macro using concatenation operator required for Rhapsody compatibility. */   
#define GET_OPORT(PORT, INTERFACE)    \
    (me->PORT.outBound._##INTERFACE)
/* #MISRA_D4.9,20.10=ON (end of ID-0008) */

#endif
