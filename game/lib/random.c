#include "../include/common.h"

static uint32_t seed = 0;

void srand(uint32_t s)
{
	seed = s;
}

uint32_t rand()
{
	seed = 0x015a4e35 * seed + 1;
	return (seed >> 16) & 0x7fff;
}
