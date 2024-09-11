/*
 * RXF_EventMacros 
 *
 * This file belongs to the Willert Software Tools RXF Product  
 * RXF_Rpy_C.
 *
 * Copyright (c) 2023, Willert Software Tools GmbH.
 *
 * E-mail: support(at)willert.de
 */


#ifndef generic_RXF_EventMacros_H
#define generic_RXF_EventMacros_H

#include "RXF_StandardTypes.h"
#include "RXF_Reactive.h"
/*## package generic */

/*## class TopLevel::RXF_EventMacros */
/*#[ ignore */
/** 
 *  RXF Class RXF_EventMacros
 *
 *  Header file containing the event generation macros.
 */ 
/*#]*/

/** 
 *  Rhapsody specific (and oxf compatible) event generation macro. A dynamic event will be created from the memory pools  
 *  and sent to the specified destination.
 */ 
#define RiCGEN CGEN

/** 
 *  Rhapsody specific (and oxf compatible) event generation macro. A dynamic event will be created from the memory pools  
 *  and sent to the specified destination via an interface.
 */ 
#define RiCGEN_I CGEN_I

/** 
 *  Rhapsody specific (and oxf compatible) event generation macro. A static event will be sent to the specified destination.  
 *  This can be used from an interrupt service routine, but it may also be used from another context where a static event  
 *  shall be sent.
 */ 
#define RiCGEN_ISR CGEN_ISR

/** 
 *  Rhapsody specific (and oxf compatible) event generation macro. A dynamic event will be created from the memory pools  
 *  and sent to the specified destination.
 */ 
#define RiCGEN_O CGEN_O
/***    User explicit entries    ***/


/* Operations */


/** 
 *  Event generation macro. A dynamic event will be created from the memory pools and sent to the specified  
 *  destination.
 *
 *    DESTINATION (In): Destination object that shall receive the event
 *  
 *    EVENT (In): Event name followed by "(<with optional arguments>)"
 *  
 */ 
/*## operation CGEN(MacroArg_t,MacroArg_t) */
/* #MISRA_D4.9,20.10=OFF ID-0008: Function-like macro using concatenation operator required for Rhapsody compatibility. */   
#define CGEN(DESTINATION, EVENT)    \
    if( NULL != (DESTINATION) )\
    {\
    	RXF_Reactive* destination = &((DESTINATION)->ric_reactive);\
    	RXF_Event* event = (RXF_Event*)RiC_Create_##EVENT;\
    	RXF_Reactive_gen( destination, event RXF_MONITOR_GEN_SOURCE_ARGUMENT );\
    }
/* #MISRA_D4.9,20.10=ON (end of ID-0008) */


/** 
 *  RXF specific event generation macro. A dynamic event will be created from the memory pools and sent to the specified  
 *  destination object.
 *
 *    DESTINATION (In): Destination object that shall receive the event
 *  
 *    EVENT (In): Event name followed by "(<with optional arguments>)"
 *  
 */ 
/*## operation CGEN_O(MacroArg_t,MacroArg_t) */
/* #MISRA_D4.9,20.10=OFF ID-0008: Function-like macro using concatenation operator required for Rhapsody compatibility. */   
#define CGEN_O(DESTINATION, EVENT)    \
    {\
    	RXF_Reactive* destination = &((DESTINATION).ric_reactive);\
    	RXF_Event* event = (RXF_Event*)RiC_Create_##EVENT;\
    	RXF_Reactive_gen( destination, event RXF_MONITOR_GEN_SOURCE_ARGUMENT );\
    }
/* #MISRA_D4.9,20.10=ON (end of ID-0008) */


/** 
 *  Event generation macro. A dynamic event will be created from the memory pools and sent to the specified  
 *  destination via an interface.
 *
 *    INSTANCE (In): Interface destination that shall receive the event
 *  
 *    EVENT (In): Event name followed by "(<with optional arguments>)"
 *  
 */ 
/*## operation CGEN_I(MacroArg_t,MacroArg_t) */
/* #MISRA_D4.9,20.10=OFF ID-0008: Function-like macro using concatenation operator required for Rhapsody compatibility. */   
#define CGEN_I(INSTANCE, EVENT)    \
    if( NULL != (INSTANCE) )\
    {\
    	RXF_Reactive* destination = (INSTANCE)->ric_reactive;\
    	RXF_Event* event = (RXF_Event*)RiC_Create_##EVENT;\
    	RXF_Reactive_gen( destination, event RXF_MONITOR_GEN_SOURCE_ARGUMENT );\
    }
/* #MISRA_D4.9,20.10=ON (end of ID-0008) */


/** 
 *  Event generation macro. A static event will be sent to the specified destination. This can be used from an  
 *  interrupt service routine, but it may also be used from another context where a static event shall be sent.
 *
 *    DESTINATION (In): Destination object that shall receive the event
 *  
 *    EVENT (In): Pointer to a static event structure that has already been initialized
 *  
 */ 
/*## operation CGEN_ISR(MacroArg_t,MacroArg_t) */
/* #MISRA_D4.9=OFF ID-0007: Function-like macro required for Rhapsody compatibility. */   
#define CGEN_ISR(DESTINATION, EVENT)    \
    if( NULL != (DESTINATION) )\
    {\
    	RXF_Reactive* destination = &((DESTINATION)->ric_reactive);\
    	RXF_Event* event = &((EVENT)->ric_event);\
    	RXF_Reactive_gen( destination, event RXF_MONITOR_GEN_SOURCE_ARGUMENT);\
    }									
/* #MISRA_D4.9=ON (end of ID-0007) */


/** 
 *  Event generation macro. A static event will be sent to the specified destination object. This can be used from an  
 *  interrupt service routine, but it may also be used from another context where a static event shall be sent.
 *
 *    DESTINATION (In): Destination object that shall receive the event
 *  
 *    EVENT (In): Pointer to a static event structure that has already been initialized
 *  
 */ 
/*## operation CGEN_ISR_O(MacroArg_t,MacroArg_t) */
/* #MISRA_D4.9=OFF ID-0007: Function-like macro required for Rhapsody compatibility. */   
#define CGEN_ISR_O(DESTINATION, EVENT)    \
    {\
    	RXF_Reactive* destination = &((DESTINATION).ric_reactive);\
    	RXF_Event* event = &((EVENT)->ric_event);\
    	RXF_Reactive_gen( destination, event RXF_MONITOR_GEN_SOURCE_ARGUMENT);\
    }									
/* #MISRA_D4.9=ON (end of ID-0007) */


/** 
 *  RXF specific event generation macro. A static event will taken from an event array (see also RXF_Event_initStaticArray())  
 *  and sent to the specified destination. This can be used from an interrupt service routine, but it may also be used from  
 *  another context where a static event shall be sent.
 *
 *    DESTINATION (In): Destination object that shall receive the event
 *  
 *    EVENT_ARRAY (In): Array of static events that has previously been initializes using RXF_Event_initStaticArray()
 *  
 *    INDEX_ATTR (In): Attribute or variable to store the current array index
 *  
 *    SIZE (In): Number of elements of the static event array
 *  
 */ 
/*## operation CGEN_ISR_ARRAY(MacroArg_t,MacroArg_t,MacroArg_t,MacroArg_t) */
/* #MISRA_D4.9=OFF ID-0007: Function-like macro required for Rhapsody compatibility. */   
#define CGEN_ISR_ARRAY(DESTINATION, EVENT_ARRAY, INDEX_ATTR, SIZE)    \
    if( (INDEX_ATTR) >= (SIZE) )\
    {\
      (INDEX_ATTR) = 0U;\
    }\
    if( NULL != (DESTINATION) )\
    {\
    	RXF_Reactive* destination = &((DESTINATION)->ric_reactive);\
    	RXF_Event* event = &((EVENT_ARRAY)[(INDEX_ATTR)].ric_event);\
    	RXF_Reactive_gen( destination, event RXF_MONITOR_GEN_SOURCE_ARGUMENT);\
    }\
    (INDEX_ATTR)++;
/* #MISRA_D4.9=ON (end of ID-0007) */


/** 
 *  RXF specific event generation macro. A static event will be sent to the specified destination. This can be used from an  
 *  interrupt service routine, but it may also be used from another context where a static event shall be sent.
 *
 *    EVENT (In): Destination object that shall receive the event
 *  
 *    EVENT_ARRAY (In): Pointer to a static event structure that has already been initialized
 *  
 *    SIZE (In)
 *  
 */ 
/*## operation RXF_Event_initStaticArray(MacroArg_t,MacroArg_t,MacroArg_t) */
/* #MISRA_D4.9,20.10=OFF ID-0008: Function-like macro using concatenation operator required for Rhapsody compatibility. */   
#define RXF_Event_initStaticArray(EVENT, EVENT_ARRAY, SIZE)    \
    {\
    	uint32_t index;																	\
    	for( index = 0U; index < (SIZE); index++ )								\
    	{																										\
    	  EVENT##_Init( &((EVENT_ARRAY)[index]) );								\
    	  RXF_Event_setDeleteAfterConsume( &((EVENT_ARRAY)[index].ric_event), false );										\
    	}\
    }																			\
    
/* #MISRA_D4.9,20.10=ON (end of ID-0008) */

#endif
