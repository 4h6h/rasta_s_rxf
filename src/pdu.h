#ifndef PDU_H
#define PDU_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAFETY_CODE_LENGTH          8U
#define PDU_FIXED_FIELDS_LENGTH     36U
#define MAX_PDU_LENGTH              50U
#define CONN_REQ_PAYLOAD_LENGTH     14U
#define CONN_RESP_PAYLOAD_LENGTH    14U
#define DISC_REQ_PAYLOAD_LENGTH     4U

#define SENDER_ID   0x00000000U
#define RECEIVER_ID 0x00000000U

#define N_SEND_MAX  0xFFFFU

#define PROTOCOL_VERSION    0x30333031U /* Protocol version, for which all 4 bytes are decimal digits in ASCII, 
                                        e. g.: “0301” = 0x30 0x33 0x30 0x31 = Version 03.01 */

#define SHIFT_1_BYTES  8U
#define SHIFT_2_BYTES  16U
#define SHIFT_3_BYTES  24U

/* Enum for message types */
typedef enum {
    MSGT_CONNECTION_REQUEST = 6200U,
    MSGT_CONNECTION_RESPONSE = 6201U,
    MSGT_RETRANSMISSION_REQUEST = 6212U,
    MSGT_RETRANSMISSION_RESPONSE = 6213U,
    MSGT_DISCONNECTION_REQUEST = 6216U,
    MSGT_HEARTBEAT = 6220U,
    MSGT_DATA = 6240U,
    MSGT_RETRANSMITTED_DATA = 6241U,
} MessageType;

typedef enum {
    MSGT_LENGTH_CONNECTION_REQUEST = 50U,
    MSGT_LENGTH_CONNECTION_RESPONSE = 50U,
    MSGT_LENGTH_RETRANSMISSION_REQUEST = 36U,
    MSGT_LENGTH_RETRANSMISSION_RESPONSE = 36U,
    MSGT_LENGTH_DISCONNECTION_REQUEST = 40U,
    MSGT_LENGTH_HAERTBEAT = 36U,
    MSGT_LENGTH_DATA = 28U + 8U,
    MSGT_LENGTH_RETRANSMISSION_DATA = 28U + 8U,
} MessageTypeLength;

/* PDU structure */
typedef struct PDU_S {
    uint16_t message_length;
    MessageType message_type;
    uint32_t receiver_id;
    uint32_t sender_id;
    uint32_t sequence_number;
    uint32_t confirmed_sequence_number;
    uint32_t timestamp;
    uint32_t confirmed_timestamp;
    const uint8_t *payload;
    const uint8_t *safety_code;
} PDU_S;

typedef enum {
    DISC_REASON_USER_REQUEST = 0U,
    DISC_REASON_UNDEFINED_MSG_TYPE_RECV,
    DISC_REASON_NOT_EXPECTED_RECV_MSG_TYPE,
    DISC_REASON_SEQ_NBR_ERR_FOR_CONNECTION,
    DISC_REASON_TIMEOUT_INCOMING_MSG,
    DISC_REASON_STATE_SERVICE_NOT_ALLOWED,
    DISC_REASON_PROTOCOL_VERSION_ERROR,
    DISC_REASON_FAIL_RETRANSMISSION,
    DISC_REASON_SEQ_ERR,
} DiscReasonType;

/**
 * @brief Serialize fields in to a buffer with data from PDU structure.
 *
 * @param[in]   pdu         Protocol Data Unit (PDU_S) structure.
 * @param[out]  buffer      Buffer that will be serialized with PDU_S structure.
 * @param[in]   buffer_size The size of the buffer (PDU_FIXED_FIELDS_LENGTH + payload length). Payload length depends on message type.
 */
void serialize_pdu(PDU_S *pdu, uint8_t *buffer, const size_t buffer_size);

/**
 * @brief Deserialize data in to the PDU structure from a buffer with serialized data.
 *
 * @param[in]   buffer      Buffer that will be deserialized in to the PDU_S structure.
 * @param[in]   buffer_size The size of the buffer (PDU_FIXED_FIELDS_LENGTH + payload length). Payload length depends on message type. 
 * @param[out]  pdu         Protocol Data Unit (PDU_S) structure.
 */
void deserialize_pdu(const uint8_t* const buffer, const size_t buffer_size, PDU_S *pdu);

/**
 * @brief Create PDU for Connection Request.
 * 
 * @param[out]  ret_pdu     The returned Connection Request PDU.
 */
void conn_req_pdu(PDU_S *ret_pdu);

/**
 * @brief Create PDU for Connection Response.
 * 
 * @param[out]  ret_pdu     The returned Connection Response PDU.
 */
void conn_resp_pdu(PDU_S *ret_pdu);

/**
 * @brief Create PDU for Retransmission Request.
 * 
 * @param[out]  ret_pdu     The returned Retransmission Request PDU.
 */
void retr_req_pdu(PDU_S *ret_pdu);

/**
 * @brief Create PDU for Retransmission Response.
 * 
 * @param[out]  ret_pdu     The returned Retransmission Response PDU.
 */
void retr_resp_pdu(PDU_S *ret_pdu);

/**
 * @brief Create PDU for Disconnection Request.
 * 
 * @param[in]   discReason  Disconnection reason.
 * @param[out]  ret_pdu     The returned Disconnection Request PDU.
 */
void disc_req_pdu(DiscReasonType discReason, PDU_S *ret_pdu);


/**
 * @brief Create PDU for Heartbeat.
  * 
 * @param[out]  ret_pdu     The returned Heartbeat PDU.
 */
void hb_pdu(PDU_S *ret_pdu);

#endif // PDU_H
