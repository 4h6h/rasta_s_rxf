#include "iDisp.h"

/*## class iDisp */
void iDisp_Cleanup(iDispRastaS* const me) {
}

/*## operation */
StdRet_t iRastaS_ReceiveSpdu(iDispRastaS* const me, const ConnId_t connId, const SpduLen_t spduLen, const uint8_t* const pSpduData) {
    return (me->iDispRastaSVtbl->fp_iRastaS_ReceiveSpdu)( me->realMe, connId, spduLen, pSpduData );
}

void iRastaS_Main(iDispRastaS* const me) {
    return (me->iDispRastaSVtbl->fp_iRastaS_Main)(me->realMe);
}

void iFec_Main(iDispFec* const me) {
    return (me->iDispFecVtbl->fp_iFec_Main)(me->realMe);
}