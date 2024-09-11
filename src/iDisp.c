#include "iDisp.h"

/*## class iDisp */
void iDisp_Cleanup(iDisp* const me) {
}

/*## operation */
StdRet_t iRastaS_ReceiveSpdu(iDisp* const me, const ConnId_t connId, const SpduLen_t spduLen, const uint8_t* const pSpduData) {
    return (me->iDispVtbl->fp_iRastaS_ReceiveSpdu)( me->realMe, connId, spduLen, pSpduData );
}
