#ifndef __IRQ_H__
#define __IRQ_H__

#include "types.h"

typedef struct TrapFrame 
{
	uint32_t edi, esi, ebp, xxx, ebx, edx, ecx, eax;
	int32_t irq;
	uint32_t error_code;
} TrapFrame;



#endif
