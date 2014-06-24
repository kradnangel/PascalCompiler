.file "test/t1.s"

sys_call_id = 0x80
exit_syscall = 0x1

.data
.globl cz_002
		.section .rodata
		.align 4
.LCcz_002:
		.string "Out of boundary!"
		.data
		.align 4
		.type cz_002 @object
		.size cz_002, 4
cz_002:
		.long .LCcz_002
.globl cz_001
		.section .rodata
		.align 4
.LCcz_001:
		.string "Please input 1-20"
		.data
		.align 4
		.type cz_001 @object
		.size cz_001, 4
cz_001:
		.long .LCcz_001


#---program test_case1 ---

# --- main routine ----
		.text
.globl _main
		.type _main, @function
_main:
		pushl	%ebp
		movl	%esp, %ebp
		leal	vf_001, %eax
		pushl	%eax
		movl	$1, %eax
		subl	$1, %eax
		movl	$4, %ecx
		imul	%ecx
		popl	%edx
		addl	%eax, %edx
		pushl	%edx
		movl	$1, %eax
		pushl	%eax
		popl	%eax
		popl	%ebx
		movl	%eax, (%ebx)
		leal	vf_001, %eax
		pushl	%eax
		movl	$2, %eax
		subl	$1, %eax
		movl	$4, %ecx
		imul	%ecx
		popl	%edx
		addl	%eax, %edx
		pushl	%edx
		movl	$1, %eax
		pushl	%eax
		popl	%eax
		popl	%ebx
		movl	%eax, (%ebx)
		movl	$3, %eax
		movl	%eax, vi_002
for_test_0:
		movl	vi_002, %eax
		pushl	%eax
		movl	$20, %eax
		popl	%edx
		cmpl	%eax, %edx
		movl	$1, %eax
		jle	j_001
		xorl	%eax, %eax
j_001:
		cmpl	$1, %eax
		jl	for_exit_0
		leal	vf_001, %eax
		pushl	%eax
		movl	vi_002, %eax
		subl	$1, %eax
		movl	$4, %ecx
		imul	%ecx
		popl	%edx
		addl	%eax, %edx
		pushl	%edx
		leal	vf_001, %eax
		pushl	%eax
		movl	vi_002, %eax
		pushl	%eax
		movl	$1, %eax
		popl	%edx
		subl	%eax, %edx
		movl	%edx, %eax
		subl	$1, %eax
		movl	$4, %ecx
		imul	%ecx
		popl	%edx
		addl	%eax, %edx
		pushl	%edx
		popl	%ebx
		movl	(%ebx), %eax
		pushl	%eax
		leal	vf_001, %eax
		pushl	%eax
		movl	vi_002, %eax
		pushl	%eax
		movl	$2, %eax
		popl	%edx
		subl	%eax, %edx
		movl	%edx, %eax
		subl	$1, %eax
		movl	$4, %ecx
		imul	%ecx
		popl	%edx
		addl	%eax, %edx
		pushl	%edx
		popl	%ebx
		movl	(%ebx), %eax
		popl	%edx
		addl	%edx, %eax
		pushl	%eax
		popl	%eax
		popl	%ebx
		movl	%eax, (%ebx)
		incl vi_002
		jmp for_test_0
for_exit_0:
		movl	cz_001, %eax
		pushl	%eax
		pushl	%ebp
		call	_writeln_string
		addl	$8, %esp
		leal	vi_002, %eax
		pushl	%eax
		pushl	%ebp
		call	_read_int
		addl	$8, %esp
		movl	vi_002, %eax
		pushl	%eax
		movl	$20, %eax
		popl	%edx
		cmpl	%eax, %edx
		movl	$1, %eax
		jle	j_002
		xorl	%eax, %eax
j_002:
		pushl	%eax
		movl	vi_002, %eax
		pushl	%eax
		movl	$0, %eax
		popl	%edx
		cmpl	%eax, %edx
		movl	$1, %eax
		jge	j_003
		xorl	%eax, %eax
j_003:
		popl	%edx
		andl	%edx, %eax
		cmpl	$1, %eax
		jl	if_false_0
		leal	vf_001, %eax
		pushl	%eax
		movl	vi_002, %eax
		subl	$1, %eax
		movl	$4, %ecx
		imul	%ecx
		popl	%edx
		addl	%eax, %edx
		pushl	%edx
		popl	%ebx
		movl	(%ebx), %eax
		pushl	%eax
		pushl	%ebp
		call	_writeln_int
		addl	$8, %esp
		jmp if_exit_0
if_false_0:
		movl	cz_002, %eax
		pushl	%eax
		pushl	%ebp
		call	_writeln_string
		addl	$8, %esp
if_exit_0:
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
		.comm vi_002,4,4
		.comm vf_001, 80, 4
