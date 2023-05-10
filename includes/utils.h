#pragma once

#include <3ds/types.h>

// For accessing physmem uncached (and directly)
#define PA_PTR(addr)            (void *)((u32)(addr) | 1 << 31)

#define REG32(addr)             (*(vu32 *)(PA_PTR(addr)))