#include <3ds/asminc.h>

BEGIN_ASM_FUNC initSystem, weak
	ldr	r2, =saved_stack
	str	sp, [r2]
	str	lr, [r2,#4]

	bl	__libctru_init
    bl  __appInit
	bl	__libc_init_array

	ldr	r2, =saved_stack
	ldr	lr, [r2,#4]
 	bx	lr
END_ASM_FUNC

BEGIN_ASM_FUNC __ctru_exit, weak
	bl	__libc_fini_array
    bl	__appExit

	ldr	r2, =saved_stack
	ldr	sp, [r2]
	b	__libctru_exit
END_ASM_FUNC

	.section .bss.saved_stack.42, "aw", %nobits
	.align 2
saved_stack:
	.space 8
