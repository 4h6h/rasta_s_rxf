#ifndef RASTAS_CFG
#define RASTAS_CFG

#include "RXF_StandardTypes.h"
#include "sm.h"

#define INSTNAME_LENGTH 10U
#define MAX_CONNECTIONS 1U

struct RastaSConfig {
    uint8_t instname[INSTNAME_LENGTH];
    SmRole role;
    MsgId_t max_connections;
    SmType* sms;
};
typedef struct RastaSConfig RastaSConfig;


#endif /* RASTAS_CFG */