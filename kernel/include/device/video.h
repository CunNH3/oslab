#ifndef __VIDEO_H__
#define __VIDEO_H__

#include "../common.h"

#define SCR_WIDTH		800
#define SCR_HEIGHT		600
#define SCR_DEPTH		3
#define SCR_WIDTH_SIZE	(SCR_WIDTH * SCR_DEPTH)
#define SCR_SIZE		((SCR_WIDTH) * (SCR_HEIGHT) * (SCR_DEPTH))
//#define VMEM_ADDR		0xFD000000
//uint32_t VMEM_ADDR;

#define LS_WIDTH		80
#define LS_HEIGHT		80
#define LS_WIDTH_SIZE	(LS_WIDTH * SCR_DEPTH)
#define FF_WIDTH		80
#define FF_HEIGHT		80
#define FF_WIDTH_SIZE	(FF_WIDTH * SCR_DEPTH)
#define BT_WIDTH		4
#define BT_HEIGHT		20
#define BT_WIDTH_SIZE	(BT_WIDTH * SCR_DEPTH)

union Pixels
{
	struct
	{
		uint8_t blue;
		uint8_t green;
		uint8_t red;
	};
	uint8_t RGB_array[3];
	uint32_t RGB_value : 24;
};

struct Surface
{
	int x, y;
	int w, h;
	uint8_t *pixels;
};

extern uint8_t *vmem;

void init_vmem();
void init_vmem_addr();
int display(uint8_t*);

#endif
