#ifndef cRASTASOP_IMPL_H
#define cRASTASOP_IMPL_H

#include "cRastaS.h"
#include "cFec.h"

RXFTakeEventStatus rootState_dispatchEvent_impl(void * const void_me, const RXFEventId id);

void cRastaSOP_Init_impl(cRastaSOP* const me, const struct RastaSConfig* const pConfig);
StdRet_t cRastaSOP_OpenConnection_impl(cRastaSOP* const me, const ConnId_t connId);
StdRet_t cRastaSOP_CloseConnection_impl(cRastaSOP* const me, const ConnId_t connId);
StdRet_t cRastaSOP_ReceiveSpdu_impl(cRastaSOP* const me, const ConnId_t conn_id, SpduLen_t spduLen, const uint8_t* const pSpduData);

#endif /* cRASTASOP_IMPL_H */