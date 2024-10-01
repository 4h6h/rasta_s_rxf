#ifndef ISC_CFG_H
#define ISC_CFG_H

#include "log.h"

#define LOG_LEVEL LOG_DEBUG

#define SERVER_SIMULATION
#undef SERVER_SIMULATION

typedef struct ip_table 
{
    const char *ip;
    const ConnId_t conn_id;
} ip_table;

#endif /* ISC_CFG_H */