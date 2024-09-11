#include "Ipc.h"

IpcConfig ipcConfig = {
    .mq_name_in  = "/MSGQ_ISC0",
    .mq_name_out = "/MSGQ_ISC1",
    .mq_msgsize = 1024U,
    .mq_maxmsg = 10U,
    .mq_flags = 0U,
    .mq_o_flags_in = O_RDWR | O_CREAT,
    .mq_o_flags_out = O_RDWR | O_CREAT,
    .mq_access_flags = 0777,
};

IpcTick ipcTick = {
    .tv_sec = 0U,
    .tv_usec = 10U*1000U,
};