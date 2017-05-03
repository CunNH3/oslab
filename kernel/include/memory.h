#ifndef KERN_INC_MEM
#define KREN_INC_MEM

#include "list.h"

#define MAX_MEM 0x20000000


typedef struct
{
	uint32_t base, limit, gdt;
	ListHead list;
} SegMan;

#endif 
