#include <3ds/types.h>

Result  __sync_init(void);
void    __system_initSyscalls(void);
void    __system_allocateHeaps(void);

void __libctru_init(void (*retAddr)(void))
{
    // Initialize the synchronization subsystem
    __sync_init();

    // Initialize newlib support system calls
    __system_initSyscalls();

    // Allocate application heaps
    __system_allocateHeaps();
}