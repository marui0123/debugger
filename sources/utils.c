#include <3ds/svc.h>
#include "utils.h"

void     Flash(u32 color)
{
    color |= 0x01000000;
    for (u32 i = 0; i < 64; i++)
    {
        REG32(0x10202204) = color;
        svcSleepThread(5000000);
    }
    REG32(0x10202204) = 0;
}
