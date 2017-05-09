#ifndef __IRQ_H__
#define __IRQ_H__

#include "types.h"

typedef struct TrapFrame 
{
	uint32_t edi, esi, ebp, old_esp, ebx, edx, ecx, eax;
	int32_t irq;
	uint32_t error_code, eip, cs, eflags;
} TrapFrame;

struct TrapFrameA
{
	uint32_t edi, esi, ebp, esp_;
	uint32_t ebx, edx, ecx, eax;
	uint32_t gs, fs, es, ds;

	int irq;
	uint32_t err;
	uint32_t eip;
	uint16_t cs;
	uint16_t padding3;
	uint32_t eflags;
	uint32_t esp;
	uint16_t ss;
	uint16_t padding4;
}__attribute__((packed));

typedef struct TrapFrameA TrapFrameA;

#endif
