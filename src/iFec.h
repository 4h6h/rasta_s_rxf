#ifndef iFEC_H
#define iFEC_H

#include "RXF_StandardTypes.h"
#include "RXF_RhapsodyTypes.h"
#include "RXF_MemoryManager.h"
#include "RXF_EventMacros.h"
#include "RXF_PortMacros.h"
#include "RXF_Monitor.h"

/* Forward declaration for VTBL */
struct iFec;

struct RastaSConn {

};
typedef struct RastaSConn RastaSConn;


typedef struct iFec_RastaS_Vtbl {
    StdRet_t (*fp_iRastaS_OpenConnection)(struct iFec* const me, const ConnId_t connId);
    StdRet_t (*fp_iRastaS_CloseConnection)(struct iFec* const me, const ConnId_t connId);
    StdRet_t (*fp_iRastaS_ConnectionStateRequest)(struct iFec* const me, const ConnId_t connId, struct RastaSConn* const state);   
    StdRet_t (*fp_iRastaS_SendData)(struct iFec* const me, const ConnId_t connId, const MsgLen_t msgLen, const uint8_t* const pMsgData); 
} iFec_RastaS_Vtbl;

/*## class iFec */
typedef struct iFec iFec;
struct iFec {
    const iFec_RastaS_Vtbl * iFecVtbl;
    iFec* realMe;
};

/***    User implicit entries    ***/


/* Constructors and destructors:*/

void iFec_Cleanup(iFec* const me);

/***    User explicit entries    ***/

/* Operations */
StdRet_t iRastaS_OpenConnection(iFec* const me, const ConnId_t connId);
StdRet_t iRastaS_CloseConnection(iFec* const me, const ConnId_t connId);
StdRet_t iRastaS_ConnectionStateRequest(iFec* const me, const ConnId_t connId, struct RastaSConn* const state);
StdRet_t iRastaS_SendData(iFec* const self, const ConnId_t connId, const MsgLen_t msgLen, const uint8_t* const pMsgData);

#define Rass_OpenConnection iRastaS_OpenConnection
#define Rass_CloseConnection iRastaS_CloseConnection
#define Rass_ConnectionStateRequest iRastaS_ConnectionStateRequest
#define Rass_SendDate iRastaS_SendData

#endif /* iFEC_HE */