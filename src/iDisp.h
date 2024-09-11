#ifndef iDISP_H
#define iDISP_H

#include "RXF_StandardTypes.h"
#include "RXF_RhapsodyTypes.h"
#include "RXF_MemoryManager.h"
#include "RXF_EventMacros.h"
#include "RXF_PortMacros.h"
#include "RXF_Monitor.h"

/* Forward declaration for VTBL */
struct iDisp;

typedef struct iDisp_Vtbl {
    StdRet_t (*fp_iRastaS_ReceiveSpdu)(struct iDisp* const me, const ConnId_t connId, const SpduLen_t msgLen, const uint8_t* const pMsgData); 
} iDisp_Vtbl;

/*## class iFec */
typedef struct iDisp iDisp;
struct iDisp {
    const iDisp_Vtbl * iDispVtbl;
    iDisp* realMe;
};

/***    User implicit entries    ***/


/* Constructors and destructors:*/

void iDisp_Cleanup(iDisp* const me);

/***    User explicit entries    ***/


/* Operations */

StdRet_t iRastaS_ReceiveSpdu(iDisp* const me, const ConnId_t connId, const SpduLen_t spduLen, const uint8_t* const pSpduData);

#define Rass_ReceiveSpdu iRastaS_ReceiveSpdu

#endif /* iDISP_H */