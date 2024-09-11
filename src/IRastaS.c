#include "iRastaS.h"

void iRastaS_Fec_Cleanup(iRastaSFec* const me) {

};

void iRastaS_Disp_Cleanup(iRastaSDisp* const me) {
    
};

/* Operations */
StdRet_t iDisp_SendSpdu(struct iRastaSDisp* const me, const ConnId_t connId, const SpduLen_t spduLen, const uint8_t* const pSpduData) {
    return me->iRastaSDispVtbl->fp_iDisp_SendSpdu(me, connId, spduLen, pSpduData);
};

StdRet_t iFec_ReceiveMsg(struct iRastaSFec* const me, const ConnId_t connId, const MsgLen_t msgLen, const uint8_t* const pMsgData) {
    return me->iRastaSFecVtbl->fp_iFec_ReceiveMsg(me, connId, msgLen, pMsgData);
};

StdRet_t iFec_ConnStateNotification(struct iRastaSFec* const me, const ConnId_t connId, const struct RastaSConn* const state) {
    return me->iRastaSFecVtbl->fp_iFec_ConnStateNotification(me, connId, state);
};
