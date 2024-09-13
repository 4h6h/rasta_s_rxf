#ifndef cDispOP_H
#define cDispOP_H

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

#include "iDisp.h"
#include "iRastaS.h"


/*## class cDispOP */
typedef struct cDispOP cDispOP;

/*## Port p_Interface_out outBound declaration. */
typedef struct {
    iDispRastaS* _iDispRastaS;
    iDispFec* _iDispFec;
} cDispOP_p_Interface_out;

/*## Port p_Interface_in inBound declaration. */
typedef struct {
	 iRastaSDisp* _iRastaSDisp;
} cDispOP_p_Interface_in;

/*## Port p_Interface declaration. */
typedef struct {
	cDispOP_p_Interface_out outBound;
	cDispOP_p_Interface_in inBound;
} cDispOP_p_Interface;

struct cDispOP {
    RXF_Reactive ric_reactive;
    struct iRastaSDisp _RastaSDisp;
    cDispOP_p_Interface p_Interface;
};

/***    User implicit entries    ***/


/* Constructors and destructors:*/

void cDispOP_Init(cDispOP* const me);

void cDispOP_Cleanup(cDispOP* const me);

void cDispOP_Main(cDispOP* const me);

RiCBoolean cDispOP_startBehavior(cDispOP* const me);

/***    User explicit entries    ***/


/* Operations */
StdRet_t cDispOP_SendSpdu(cDispOP* const me, const ConnId_t connId, const SpduLen_t spduLen, const uint8_t* const pSpduData);

/*## operations */

cDispOP * cDispOP_Create(void);
void cDispOP_Destroy(cDispOP* const me);

/***    Framework entries    ***/

void cDispOP_p_Interface_connectOutBound_RastaS(cDispOP* const me, iDispRastaS* const p_iDisp);
void cDispOP_p_Interface_connectOutBound_Fec(cDispOP* const me, iDispFec* const p_iDisp);

#endif /* cDispOP.h */
