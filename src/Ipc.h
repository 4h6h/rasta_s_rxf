#ifndef IPC_H
#define IPC_H

#include "unistd.h"
#include "mqueue.h"

#include "RXF_StandardTypes.h"
#include "Ipc_Cfg.h"

struct IpcConfig {
    const char const mq_name_in[MQ_NAME_SIZE];
    const char const mq_name_out[MQ_NAME_SIZE];
    const uint32_t mq_flags;
    const uint16_t mq_msgsize;
    const uint16_t mq_maxmsg;
    const uint32_t mq_o_flags_in;
    const uint32_t mq_o_flags_out;
    const uint32_t mq_access_flags;
};
typedef struct IpcConfig IpcConfig;

struct IpcTick {
    const uint32_t tv_sec;
    const uint32_t tv_usec;    
};
typedef struct IpcTick IpcTick;

enum mesg_type {
    LST = 0,
    CCX,
};

typedef uint8_t mesg_type_t;

struct __attribute__((__packed__)) _ipc_message {
    mesg_type_t    type;
    uint8_t        payload[MAX_MSGQ_PAYLOAD-sizeof(mesg_type_t)];
};
typedef struct _ipc_message ipc_message_t;


/* PUBLIC methods */
StdRet_t Ipc_init(void);
void Ipc_executeOneEvent(void);

#endif /* IPC_H */