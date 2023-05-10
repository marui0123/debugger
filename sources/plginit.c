#include <3ds.h>

#include "plginit.h"
#include "plgldr.h"
#include "csvc.h"
#include "utils.h"

static void     Flash(u32 color)
{
    color |= 0x01000000;
    for (u32 i = 0; i < 64; i++)
    {
        REG32(0x10202204) = color;
        svcSleepThread(5000000);
    }
    REG32(0x10202204) = 0;
}

void    threadMain(void *arg __attribute__((unused)))
{
    initSystem(NULL);

    // Initialize services
    PLGLDR_Init();
    srvInit();
    hidInit();

    Handle memLayoutChanged;
    svcControlProcess(CUR_PROCESS_HANDLE, PROCESSOP_GET_ON_MEMORY_CHANGE_EVENT, (u32)&memLayoutChanged, 0);

    while (true)
    {
        if (svcWaitSynchronization(memLayoutChanged, 250000000ULL) == 0x09401BFE) // 0.25s
        {
            s32 event = PLGLDR_FetchEvent();
            hidScanInput();

            u32 kDown = hidKeysDown();

            if (kDown & KEY_START)
                Flash(0xFF0000);

            if (event == PLG_ABOUT_TO_EXIT)
            {
                Flash(0xFF);
                hidExit();
                srvExit();
                PLGLDR_Reply(event);
            }
        }
    }
}