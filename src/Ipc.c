#include <sys/ipc.h>
#include <sys/select.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include "log.h"

#include "Ipc.h"
#include <sys/stat.h>
#include <sys/types.h>

static mqd_t msq_in_id = -1;
static mqd_t msq_out_id = -1;
static fd_set readfds; /* set of socket descriptors */

extern IpcConfig ipcConfig;
extern IpcTick ipcTick;

StdRet_t Ipc_init(void)
{
    StdRet_t returnValue=OK;
    struct mq_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.mq_msgsize = ipcConfig.mq_msgsize;
    attr.mq_flags = ipcConfig.mq_flags;
    attr.mq_maxmsg = ipcConfig.mq_maxmsg;

    msq_in_id = mq_open(ipcConfig.mq_name_in, ipcConfig.mq_o_flags_in, ipcConfig.mq_access_flags, &attr);
    if(msq_in_id < 0)
    {   
        LOG_ERROR("could not open message queue: error code %i", msq_in_id);
        returnValue = NOT_OK;
    }

    attr.mq_flags = ipcConfig.mq_o_flags_out;
    msq_out_id = mq_open(ipcConfig.mq_name_out, ipcConfig.mq_o_flags_out, ipcConfig.mq_access_flags, &attr);
    if(msq_out_id < 0)
    {   
        LOG_ERROR("could not open message queue: error code %i", msq_out_id);
        returnValue = NOT_OK;
    }

    return returnValue;
}

static void tick_timeout_expired(const struct timeval* const tv) {
    LOG_INFO("timeout experied %u %u", (uint32_t)tv->tv_sec, (uint32_t)tv->tv_usec);
}

void Ipc_executeOneEvent(void)
{
    int64_t activity = 0U; 
    struct timeval tick = { 
        .tv_sec = ipcTick.tv_sec,
        .tv_usec = ipcTick.tv_usec,
    };

    /* clear socket set */
    FD_ZERO(&readfds);

    /* add queue fd to descriptor list */
    FD_SET(msq_in_id, &readfds);
    
    /* blocking call */
    activity = select(FD_SETSIZE, &readfds, NULL, NULL, &tick);
    if ((activity<0) && (errno!=EINTR)) {
        /* error case */
    }

    if (activity==0) {
        /* timeout tick */
        tick_timeout_expired(&tick);

    } else if (FD_ISSET(msq_in_id, &readfds)) {
        ipc_message_t message;

        /* valid ipc entry */
        printf("message received\n");
        LOG_INFO("timeout rest %u %u", (uint32_t)tick.tv_sec, (uint32_t)tick.tv_usec);
        if ( mq_receive(msq_in_id, (uint8_t *)&message, MAX_MSGQ_PAYLOAD, NULL) < 0)
        {
            perror("mq_receive failed");
            exit( 0 );
        }
        printf("%s\n", message.payload);
        mq_close(msq_in_id);
        mq_close(msq_out_id);
        exit(0);

    } else {
        /* something else */
    }

}