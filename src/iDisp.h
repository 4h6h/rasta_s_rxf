#ifndef iDISP_H
#define iDISP_H

#include "RXF_StandardTypes.h"
#include "RXF_RhapsodyTypes.h"
#include "RXF_MemoryManager.h"
#include "RXF_EventMacros.h"
#include "RXF_PortMacros.h"
#include "RXF_Monitor.h"

/* Forward declaration for VTBL */
struct iDispRastaS;
struct iDispFec;

typedef struct iDisp_RastaS_Vtbl {
    StdRet_t (*fp_iRastaS_ReceiveSpdu)(struct iDispRastaS* const me, const ConnId_t connId, const SpduLen_t msgLen, const uint8_t* const pMsgData); 
    void (*fp_iRastaS_Main)(struct iDispRastaS* const me);
} iDisp_RastaS_Vtbl;

typedef struct iDisp_Fec_Vtbl {
    void (*fp_iFec_Main)(struct iDispFec* const me);
} iDisp_Fec_Vtbl;


typedef struct iDispRastaS iDispRastaS;
struct iDispRastaS {
    const iDisp_RastaS_Vtbl* iDispRastaSVtbl;
    iDispRastaS* realMe;
};

typedef struct iDispFec iDispFec;
struct iDispFec {
    const iDisp_Fec_Vtbl* iDispFecVtbl;
    iDispFec* realMe;
};



/***    User implicit entries    ***/


/* Constructors and destructors:*/

void iDisp_Cleanup(iDispRastaS* const me);

/***    User explicit entries    ***/


/* Operations */

StdRet_t iRastaS_ReceiveSpdu(iDispRastaS* const me, const ConnId_t connId, const SpduLen_t spduLen, const uint8_t* const pSpduData);
void iRastaS_Main(iDispRastaS* const me);
void iFec_Main(iDispFec* const me);

#define Rass_ReceiveSpdu iRastaS_ReceiveSpdu
#define Rass_Main iRastaS_Main
#define Fec_Main iFec_Main

#endif /* iDISP_H */