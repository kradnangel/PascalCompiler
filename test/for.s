.file "test/for.s"

sys_call_id = 0x80
exit_syscall = 0x1

.data
.globl cz_001
		.section .rodata
		.align 4
.LCcz_001:
		.string "d = "
		.data
		.align 4
		.type cz_001 @object
		.size cz_001, 4
cz_001:
		.long .LCcz_001


#---program helloworld ---

# --- main routine ----
		.text
.globl _main
		.type _main, @function
_main:
		pushl	%ebp
		movl	%esp, %ebp
		movl	$30, %eax
		movl	%eax, vb_002
		movl	$20, %eax
		movl	%eax, vc_003
		movl	$0, %eax
		movl	%eax, vd_004
		movl	vb_002, %eax
		movl	%eax, va_001
for_test_0:
		movl	va_001, %eax
		pushl	%eax
		movl	vc_003, %eax
		popl	%edx
		cmpl	%eax, %edx
		movl	$1, %eax
		jge	j_001
		xorl	%eax, %eax
j_001:
		cmpl	$1, %eax
		jl	for_exit_0
		movl	vd_004, %eax
		pushl	%eax
		movl	va_001, %eax
		popl	%edx
		addl	%edx, %eax
		movl	%eax, vd_004
decl va_001
		jmp for_test_0
for_exit_0:
		movl	cz_001, %eax
		pushl	%eax
		pushl	%ebp
		call	_writeln_string
		addl	$8, %esp
		movl	vd_004, %eax
		pushl	%eax
		pushl	%ebp
		call	_writeln_int
		addl	$8, %esp
		leave
		ret


.globl _start
_start:
		call _main
		movl $0, %ebx
		movl $exit_syscall, %eax
		int  $sys_call_id
.ident	"SPL: 0.1.5"

#.bss variables
		.comm vd_004,4,4
		.comm vc_003,4,4
		.comm vb_002,4,4
		.comm va_001,4,4
