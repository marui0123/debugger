#include <3ds/asminc.h>

@---------------------------------------------------------------------------------
@ 3DS processor selection
@---------------------------------------------------------------------------------
    .cpu mpcore
@---------------------------------------------------------------------------------

@---------------------------------------------------------------------------------
    .section ".crt0", "ax"
    .global _start, __service_ptr, __apt_appid, __system_runflags
@---------------------------------------------------------------------------------
    .align 4
    .arm
@---------------------------------------------------------------------------------
_start:
    b startup

__service_ptr:
	.word 0 @ Pointer to service handle override list -- if non-NULL it is assumed that we have been launched from a homebrew launcher
__apt_appid:
	.word 0x300 @ Program APPID
__system_runflags:
	.word 0 @ Flags to signal runtime restrictions to ctrulib

BEGIN_ASM_FUNC startup, local
    @ Save return address
    mov     r4, lr

    @ Clear the BSS section
    ldr     r0, =__bss_start__
    ldr     r1, =__bss_end__
    sub     r1, r1, r0
    bl      ClearMem

    @ System initialization
	mov     r0, r4
	bl      initSystem

    @ Jump to plugin init code
    ldr     r0, [sp]
    bl      main

    @ Jump back to plugin loader hook
    mov     pc, r4
END_ASM_FUNC

@---------------------------------------------------------------------------------
@ Clear memory to 0x00 if length != 0
@  r0 = Start Address
@  r1 = Length
@---------------------------------------------------------------------------------

BEGIN_ASM_FUNC ClearMem, local
@---------------------------------------------------------------------------------
    mov     r2, #3     @ Round down to nearest word boundary
    add     r1, r1, r2 @ Shouldnt be needed
    bics    r1, r1, r2 @ Clear 2 LSB (and set Z)
    bxeq    lr         @ Quit if copy size is 0

    mov     r2, #0

    b       ClrLoop
END_ASM_FUNC

BEGIN_ASM_FUNC ClrLoop, local
    stmia   r0!, {r2}
    subs    r1, r1, #4
    bne     ClrLoop

    bx      lr
END_ASM_FUNC
