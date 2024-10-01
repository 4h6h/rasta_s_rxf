#ifndef RXF_SM_EVENT_H
#define RXF_SM_EVENT_H

#include "RXF_StandardTypes.h"
#include "RXF_Event.h"
#include "RXF_Reactive.h"

#include "sm.h"

#define RXF_MAX_SM_EVENTS 100U

#define RXF_Event_Sm_in_id  2567U
#define RXF_Event_Sm_out_id 2568U
#define RXF_Event_Sm_state_id 2569U

/* Forward declaration */

struct RXF_SM_Event {
    /** 
     *  A timeout consists of an event plus additional attributes.
     */ 
    struct RXF_Event event;		/*## attribute event */
    
    /** 
     *  Pointer to the previous event inside the busy list.
     */ 
    struct RXF_SM_Event* previous;		/*## attribute previous */

    ConnId_t conn_id;

    Event sm_event;

    const uint8_t* data;
};
typedef struct RXF_SM_Event RXF_SM_Event;

void RXF_SM_Event_gen(RXF_Reactive* const dest, const RXFEventId event_id, const ConnId_t conn_id, const Event event, const uint8_t* const data);
void RXF_SM_Event_free(const RXF_SM_Event* const ev);

#endif /* RXF_SM_EVENT_H */