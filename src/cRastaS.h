#ifndef cRastaSOP_H
#define cRastaSOP_H

#include "RXF_StandardTypes.h"
#include "RXF_RhapsodyTypes.h"
#include "RXF_MemoryManager.h"
#include "RXF_EventMacros.h"
#include "RXF_PortMacros.h"
#include "RXF_Monitor.h"

#include "RXF_VirtualTable.h"
#include "RXF_TimerManager.h"
#include "RXF_Startup.h"
#include "RXF_Event.h"

#include "iFec.h"
#include "iDisp.h"
#include "iRastaS.h"


enum cRastaSOP_Enum {
    cRastaSOP_RiCNonState = 0,
    cRastaSOP_S_WAITING_FOR_PING = 1,
    cRastaSOP_S_DELAYING_PONG = 2
};

/*## class cRastaSOP */
typedef struct cRastaSOP cRastaSOP;

/*## Port p_Interface_out outBound declaration. */
typedef struct {
    iRastaSFec* _iRastaSFec;
    iRastaSDisp* _iRastaSDisp;
} cRastaSOP_p_Interface_out;

/*## Port p_Interface_in inBound declaration. */
typedef struct {
    iFec* _iFec;    
    iDisp* _iDisp;
} cRastaSOP_p_Interface_in;

/*## Port p_Interface declaration. */
typedef struct {
	cRastaSOP_p_Interface_out outBound;
	cRastaSOP_p_Interface_in inBound;
} cRastaSOP_p_Interface;

struct cRastaSOP {
    RXF_Reactive ric_reactive;
    struct iFec _iFec;
    struct iDisp _iDisp;
    enum cRastaSOP_Enum rootState_subState;
    enum cRastaSOP_Enum rootState_active;
    cRastaSOP_p_Interface p_Interface;  
};

/***    User implicit entries    ***/

/* Constructors and destructors:*/

void cRastaSOP_Init(cRastaSOP* const me);

void cRastaSOP_Cleanup(cRastaSOP* const me);

void cRastaSOP_Main(cRastaSOP* const me);

RiCBoolean cRastaSOP_startBehavior(cRastaSOP* const me);


/***    User explicit entries    ***/

/* Operations */

/*## operations */
StdRet_t cRastaSOP_OpenConnection(cRastaSOP* const me, const ConnId_t connId);
StdRet_t cRastaSOP_CloseConnection(cRastaSOP* const me, const ConnId_t connId);
StdRet_t cRastaSOP_ConnectionStateRequest(cRastaSOP* const me, const ConnId_t connId, struct RastaSConn* const state);
StdRet_t cRastaSOP_SendData(cRastaSOP* const me, const ConnId_t connId, MsgLen_t msgLen, const uint8_t* const pMsgData);

StdRet_t cRastaSOP_ReceiveSpdu(cRastaSOP* const me, const ConnId_t connId, SpduLen_t spduLen, const uint8_t* const pSpduData);

cRastaSOP * cRastaSOP_Create(const struct RastaSConfig* const pConfig);
void cRastaSOP_Destroy(cRastaSOP* const me);

/***    Framework entries    ***/

void cRastaSOP_p_Interface_connectOutBound_Fec(cRastaSOP* const me, iRastaSFec* const p_iRastaS);
void cRastaSOP_p_Interface_connectOutBound_Disp(cRastaSOP* const me, iRastaSDisp* const p_iRastaS);


/* S_WAITING_FOR_PING: */
#define cRastaSOP_S_WAITING_FOR_PING_IN(me)    \
    ((me)->rootState_subState == cRastaSOP_S_WAITING_FOR_PING)

/* S_DELAYING_PONG: */
#define cRastaSOP_S_DELAYING_PONG_IN(me)    \
    ((me)->rootState_subState == cRastaSOP_S_DELAYING_PONG)

#endif /* cRastaSOP_H */