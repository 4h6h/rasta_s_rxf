#include "RastaS_Cfg.h"

RastaSConfig rastaConfig_client = {
    .instname = "client\0",
    .max_connections = MAX_CONNECTIONS,
    .role = ROLE_CLIENT,
};

RastaSConfig rastaConfig_server = {
    .instname = "server\0",
    .max_connections = MAX_CONNECTIONS,
    .role = ROLE_SERVER,
};