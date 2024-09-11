/*
 * RXF_Event 
 *
 * This file belongs to the Willert Software Tools RXF Product  
 * RXF_Rpy_C.
 *
 * Copyright (c) 2023, Willert Software Tools GmbH.
 *
 * E-mail: support(at)willert.de
 */


#include "RXF_Event.h"
#include "RXF_Reactive.h"
/*## package generic */

/*## class RXF_Event */
/*## operation Init(RXFEventId,RXF_Reactive) */
void RXF_Event_init(RXF_Event* const me, const RXFEventId eventId, struct RXF_Reactive* const destinationFSM)
{
    /* Note that the pointer arguments do not need to be tested for NULL; 
     * - the me pointer not being NULL is being tested in Rhapsody generated code in a <event>_Init() already 
     * - destinationFSM can be NULL
     */
    
    me->lId = eventId;
    me->destination = destinationFSM;      
    me->isStatic = false;
}

