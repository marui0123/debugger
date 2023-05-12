#include <3ds.h>
#include "csvc.h"
#include "plginit.h"

// Threads stacks
static u8   threadMainStack[0x1000] ALIGN(8);
Handle      threadMainHandle;

void    main(void)
{
    // Start main thread
    svcCreateThread(&threadMainHandle, threadMain, 0, (u32 *)&threadMainStack[0x1000], 0x1A, 0);
}
