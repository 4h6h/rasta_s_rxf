#ifndef iRASTAS_H
#define iRASTAS_H

#include "RXF_StandardTypes.h"
#include "RXF_RhapsodyTypes.h"
#include "RXF_MemoryManager.h"
#include "RXF_EventMacros.h"
#include "RXF_PortMacros.h"
#include "RXF_Monitor.h"

#include "RastaS_Cfg.h"

/* Forward declaration for VTBL */
struct iRastaSFec;
struct iRastaSDisp;
struct RastaSConn;
/* struct RastaSConfig; */

typedef struct iRastaS_Fec_Vtbl {
    StdRet_t (*fp_iFec_ReceiveMsg)(struct iRastaSFec* const me, const ConnId_t connId, const MsgLen_t msgLen, const uint8_t* const pMsgData);
    StdRet_t (*fp_iFec_ConnStateNotification)(struct iRastaSFec* const me, const ConnId_t connId, const struct RastaSConn* const state);
} iRastaS_Fec_Vtbl;

typedef struct iRastaS_Disp_Vtbl {
    StdRet_t (*fp_iDisp_SendSpdu)(struct iRastaSDisp* const me, const ConnId_t, const SpduLen_t connId, const uint8_t* const pSpduData);
} iRastaS_Disp_Vtbl;


/*## class iRastaS */
typedef struct iRastaSFec iRastaSFec;
struct iRastaSFec {
    const iRastaS_Fec_Vtbl* iRastaSFecVtbl;
    iRastaSFec* realMe;
};

/*## class iRastaSDisp */
typedef struct iRastaSDisp iRastaSDisp;
struct iRastaSDisp {
    const iRastaS_Disp_Vtbl* iRastaSDispVtbl;
    struct iRastaSDisp* realMe;
};

/***    User implicit entries    ***/


/* Constructors and destructors:*/

void iRastaS_Fec_Cleanup(iRastaSFec* const me);
void iRastaS_Disp_Cleanup(iRastaSDisp* const me);

/*** 
 *    User explicit entries    * 
 ***/


/* Operations */
StdRet_t iDisp_SendSpdu(struct iRastaSDisp* const me, const ConnId_t connId, const SpduLen_t spduLen, const uint8_t* const pSpduData);
StdRet_t iFec_ReceiveMsg(struct iRastaSFec* const me, const ConnId_t connId, const MsgLen_t msgLen, const uint8_t* const pMsgData);
StdRet_t iFec_ConnStateNotification(struct iRastaSFec* const me, const ConnId_t connId, const struct RastaSConn* const state);

#define Disp_SendSpdu iDisp_SendSpdu
#define Fec_ReceiveMsg iFec_ReceiveMsg
#define Fec_ConnStateNotfication iFec_ConnStateNotification


#endif /* iRASTAS_H */