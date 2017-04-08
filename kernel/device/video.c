#include "../include/device/video.h"
#include "../include/video_mode.h"
#include "../include/string.h"
#include "../include/logo.h"
uint8_t *vmem;

void init_vmem_addr()
{
	struct ModeInfoBlock *MIB = (struct ModeInfoBlock*)(0xa0000);
	vmem = (uint8_t *) MIB->physbase;
}

void init_vmem()
{
	memcpy(vmem, gImage_PARK, SCR_SIZE);
}



























