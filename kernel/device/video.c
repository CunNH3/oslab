#include "../include/device/video.h"
#include "../include/video_mode.h"
#include "../include/string.h"

uint8_t *vmem;

void init_vmem_addr()
{
	struct ModeInfoBlock *MIB = (struct ModeInfoBlock*)(0x7e00);
	vmem = (uint8_t *) MIB->physbase;
}
void init_vmem()
{
	memcpy(vmem, 0, SCR_SIZE);
}



























