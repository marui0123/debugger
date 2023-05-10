#include <3ds.h>
#include "plgldr.h"
#include <string.h>
#include "csvc.h"

static Handle   plgLdrHandle;
static Handle   plgLdrArbiter;
static s32*     plgEvent;
static s32*     plgReply;
static int      plgLdrRefCount;
static bool     isN3DS;

static Result   PLGLDR_GetArbiter(void)
{
    Result res = 0;

    u32 *cmdbuf = getThreadCommandBuffer();

    cmdbuf[0] = IPC_MakeHeader(9, 0, 0);
    if (R_SUCCEEDED((res = svcSendSyncRequest(plgLdrHandle))))
    {
        res = cmdbuf[1];
        plgLdrArbiter = cmdbuf[3];
    }
    return res;
}


Result  PLGLDR_Init(void)
{
    s64    out = 0;
    Result res = 0;

    svcGetSystemInfo(&out, 0x10000, 0x201);
    isN3DS = out != 0;
    if (AtomicPostIncrement(&plgLdrRefCount) == 0)
        res = svcConnectToPort(&plgLdrHandle, "plg:ldr");

    if (R_SUCCEEDED(res)
       && R_SUCCEEDED((res = PLGLDR_GetArbiter())))
    {
        PluginHeader *header = (PluginHeader *)0x07000000;

        plgEvent = header->plgldrEvent;
        plgReply = header->plgldrReply;
    }
    else
        PLGLDR_Exit();

    return res;
}

void    PLGLDR_Exit(void)
{
    if (AtomicDecrement(&plgLdrRefCount))
        return;

    if (plgLdrHandle)
        svcCloseHandle(plgLdrHandle);
    if (plgLdrArbiter)
        svcCloseHandle(plgLdrArbiter);
    plgLdrHandle = plgLdrArbiter = 0;
}

static s32      __ldrex__(s32 *addr)
{
    s32 val;
    do
        val = __ldrex(addr);
    while (__strex(addr, val));

    return val;
}

static void     __strex__(s32 *addr, s32 val)
{
    do
        __ldrex(addr);
    while (__strex(addr, val));
}

s32     PLGLDR_FetchEvent(void)
{
    s32  event = __ldrex__(plgEvent); // exclusive read necessary?

    return event;
}

void    PLGLDR_Reply(s32 event)
{
    __strex__(plgReply, PLG_OK);

    if (event < PLG_ABOUT_TO_SWAP)
    {
        __strex__(plgEvent, PLG_OK);
        return;
    }
    svcArbitrateAddress(plgLdrArbiter, (u32)plgReply, ARBITRATION_SIGNAL, 1, 0);
    if (event == PLG_ABOUT_TO_SWAP)
    {
        __strex__(plgEvent, PLG_WAIT);
        svcArbitrateAddress(plgLdrArbiter, (u32)plgEvent, ARBITRATION_WAIT_IF_LESS_THAN, PLG_OK, 0);
    }
    else if (event == PLG_ABOUT_TO_EXIT)
        svcExitThread();
    else
    {
        __strex__(plgEvent, PLG_OK);
    }
}