#ifndef cFecOP_H
#define cFecOP_H

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


/* */
/*## class cFecOP */
typedef struct cFecOP cFecOP;

/*## Port p_Interface_out outBound declaration. */
typedef struct {
    iFec* _iFec;
} cFecOP_p_Interface_out;

/*## Port p_Interface_in inBound declaration. */
typedef struct {
    iRastaSFec* _iRastaS;
    iDispFec* _iDisp;
} cFecOP_p_Interface_in;

/*## Port p_Interface declaration. */
typedef struct {
	cFecOP_p_Interface_out outBound;
	cFecOP_p_Interface_in inBound;
} cFecOP_p_Interface;

struct cFecOP {
    RXF_Reactive ric_reactive; 
    struct iRastaSFec _RastaSFec;
    struct iDispFec _DispFec;
    cFecOP_p_Interface p_Interface;
};

/***    User implicit entries    ***/


/* Constructors and destructors:*/

void cFecOP_Init(cFecOP* const me);

void cFecOP_Cleanup(cFecOP* const me);

void cFecOP_Main(cFecOP* const me);

RiCBoolean cFecOP_startBehavior(cFecOP* const me);

/***    User explicit entries    ***/


/* Operations */

/*## operations */
StdRet_t cFecOP_ReceiveMsg (cFecOP* const me, const ConnId_t connId, const MsgLen_t msgLen, const uint8_t* const pMsgData);
StdRet_t cFecOP_ConnStateNotification (cFecOP* const me, const ConnId_t connId, const struct RastaSConn* const state);

cFecOP * cFecOP_Create(void);
void cFecOP_Destroy(cFecOP* const me);

/***    Framework entries    ***/

void cFecOP_p_Interface_connectOutBound(cFecOP* const me, iFec* const p_iFec);

#endif /* cFecOP.h */
