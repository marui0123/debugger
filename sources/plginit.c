#include <3ds.h>

#include "plginit.h"
#include "plgldr.h"
#include "csvc.h"
#include "utils.h"

void    threadMain(void *arg __attribute__((unused)))
{
    // Initialize services
    PLGLDR_Init();

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
            {
                Flash(0xFF00);
            }

            if (event == PLG_ABOUT_TO_EXIT)
            {
                Flash(0xFF);
                PLGLDR_Reply(event);
            }
        }
    }
}
