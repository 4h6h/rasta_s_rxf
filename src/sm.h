#ifndef SM_H
#define SM_H

#include <stdint.h>
#include <stdbool.h>

#include "RXF_StandardTypes.h"
#include "RXF_Event.h"

#include "pdu.h"

#define TMAX    500U /* TODO: RTR - Define TMP_MAX */

/* Define states of the state machine */
typedef enum {
    STATE_CLOSED = 0U,
    STATE_DOWN,
    STATE_START,
    STATE_UP,
    STATE_RETR_REQ,
    STATE_RETR_RUN,
} State;

/* Define events that trigger state transitions */
typedef enum {
    EVENT_TH_ELAPSED = 0U,
    EVENT_TI_ELAPSED,
    EVENT_OPEN_CONN,
    EVENT_CLOSE_CONN,
    EVENT_SEND_DATA,
    EVENT_RECV_CONN_REQ ,
    EVENT_RECV_CONN_RESP,
    EVENT_RECV_RETR_REQ,
    EVENT_RECV_RETR_RESP,
    EVENT_RECV_DISC_REQ,
    EVENT_RECV_HB,
    EVENT_RECV_DATA,
    EVENT_RECV_RETR_DATA,
    EVENT_NO_SM_EVENT, /* last */
} Event;

typedef enum {
    ROLE_CLIENT = 0U,
    ROLE_SERVER
} SmRole;

typedef enum {
    CAT_CONN_CLOSING = 0U,
    CAT_IGNORE_EVENT,
    CAT_REGULAR_OP,
    CAT_POST_TREATMENT,
} SmCategory;


/* State machine context structure */
typedef struct SmType SmType;

/* Type definition for the state handler function pointer */
typedef void (*EventHandler)(SmType* const self, const Event event, PDU_S* const recv_pdu);

/* State machine context definition */
struct SmType {
    MsgId_t channel; /* The channel/connection/msg_id number */
    State state;     /* The state of the state machine */
    SmRole role;     /* The state machine can act like a client or like a server */
    SmCategory category; /* Action categories */
    uint32_t snr;    /* Receive sequence number (i.e. the expected sequence number of the next received protocol data unit) */
    uint32_t snt;    /* Send sequence number (i.e. the sequence number of the protocol data unit to be sent next) */
    uint32_t snpdu;  /* Sequence number field of a protocol data unit */
    uint32_t cst;    /* Sequence number to be confirmed (which is transmitted at the next protocol data unit to be sent) */
    uint32_t csr;    /* Last received confirmed sequence number */
    uint32_t tsr;    /* Timestamp of the last formally correct message received */
    uint32_t ctsr;   /* Confirmed timestamp of the last received message relevant to time monitoring */
    uint32_t cspdu;  /* Confirmed sequence number field of a protocol data unit */
    uint32_t ctspdu; /* Confirmed timestamp (field in the protocol data unit) */
    uint32_t tspdu;  /* Timestamp for the sender (field in the protocol data unit) */
    EventHandler handle_event;
};

/**
 * @brief Initializes the RastaS module.
 *
 * @param[in]   self Pointer to my RastaS structure handle.
 * 
 * @retval - `OK`      If the initialization was done successfully.
 * @retval - `NOT_OK`  If the initialization was not done successfully.
 */
StdRet_t Sm_Init(SmType* const self);

/**
 * @brief Initializes the RastaS module.
 *
 * @param[in]   self        Pointer to my RastaS structure handle.
 * @param[in]   event       Received event to handle.
 * @param[in]   recv_pdu    Received PDU to handle.
 */
void Sm_HandleEvent(SmType* const self, const Event event, PDU_S* const recv_pdu);

#endif /* SM_H */