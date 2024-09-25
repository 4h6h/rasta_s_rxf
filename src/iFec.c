#include "iFec.h"

/*## class iFec */
void iFec_Cleanup(iFec* const me) {
}

/*## operations */
StdRet_t iRastaS_OpenConnection(iFec* const me, const ConnId_t connId) {
    return (me->iFecVtbl->fp_iRastaS_OpenConnection)( me->realMe, connId );
};

StdRet_t iRastaS_CloseConnection(iFec* const me, const ConnId_t connId) {
    return (me->iFecVtbl->fp_iRastaS_CloseConnection)( me->realMe, connId );
};

StdRet_t iRastaS_ConnectionStateRequest(iFec* const me, const ConnId_t connId, struct RastaSConn* const state) {
    return (me->iFecVtbl->fp_iRastaS_ConnectionStateRequest)( me->realMe, connId, state );
};

StdRet_t iRastaS_SendData(iFec* const me, const ConnId_t connId, const MsgLen_t msgLen, const uint8_t* const pMsgData) {
    return (me->iFecVtbl->fp_iRastaS_SendData)( me->realMe, connId, msgLen, pMsgData );
};

