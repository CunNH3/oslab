#include "asm.h"
.code16
.globl start
start:
	cli

	movw    $0x13, %ax
	int     $0x10


	xorw    %ax, %ax          # %AX = 0
	movw    %ax, %ds          # %DS = AX (data segment)
	movw    %ax, %es          # %ES = AX (extra segment)
	movw    %ax, %ss          # %SS = AX (stack segment)


	movw    $0x2401, %ax
	int     $0x15

	lgdt    gdtdesc
	movl    %cr0, %eax        # %CR0 |= PROTECT_ENABLE_BIT
	orl     $0x1, %eax
	movl    %eax, %cr0

	
	ljmp    $GDT_ENTRY(1), $start32

.code32
start32:
	
	movw    $GDT_ENTRY(2), %ax
	movw    %ax, %ds          # %DS = %AX
	movw    %ax, %es          # %ES = %AX
	movw    %ax, %ss          # %SS = %AX

	
	movl    $0x8000, %esp     # %ESP = $0x8000
	call    bootmain

# GDT
.p2align 2
gdt:
	SEG_NULLASM
	SEG_ASM(0xA, 0x0, 0xffffffff)
	SEG_ASM(0x2, 0x0, 0xffffffff)

gdtdesc:
	.word   (gdtdesc - gdt - 1)
	.long   gdt

