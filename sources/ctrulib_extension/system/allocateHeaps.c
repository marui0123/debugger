#include <3ds/types.h>
#include <3ds/allocator/mappable.h>
#include "plgldr.h"

extern char*    fake_heap_start;
extern char*    fake_heap_end;
extern u32  __ctru_heap;
extern u32  __ctru_linear_heap;

u32 __ctru_heap_size        = 0;
u32 __ctru_linear_heap_size = 0;

void    __system_allocateHeaps(void)
{
    PluginHeader *header = (PluginHeader*)(0x7000000);

    __ctru_heap = header->heapVA;
    __ctru_heap_size = header->heapSize;

    mappableInit(0x11000000, 0x14000000);

    fake_heap_start = (char *)__ctru_heap;
    fake_heap_end = fake_heap_start + __ctru_heap_size;
}