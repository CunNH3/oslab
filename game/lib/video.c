#include "../include/types.h"
#include "../include/string.h"
#include "../include/video.h"
#include "../include/system.h"

#define SLOW

#ifdef SLOW
	#define PARTIAL_UPDATE
#endif

uint8_t *vmem = VMEM_ADDR;
static uint8_t vbuf[SCR_SIZE];
#ifdef PARTIAL_UPDATE
static uint8_t vref[SCR_SIZE];
#endif

void prepare_buffer(void)
{
#ifdef PARTIAL_UPDATE
	memset(vbuf,0,SCR_SIZE);
	memcpy(vref, vbuf, SCR_SIZE);
#endif
	vmem = vbuf;
	memset(vmem, 0, SCR_SIZE);
}

void display(void)
{
#ifdef PARTIAL_UPDATE
	int i;
	uint32_t *buf = (uint32_t*)vbuf;
	uint32_t *ref = (uint32_t*)vref;
	for (i = 0; i < SCR_SIZE/4 ; i ++)
	{
		if (buf[i] != ref[i])
		{
			system_draw_pixel_off(i,buf[i]);
			ref[i] = buf[i];
		}
	}
#else

	for (i = 0;i < SCR_SIZE / 4;i++)
		system_draw_pixel_off(i,buf[i]);
#endif
}

void draw_grey_screen()
{
	system_clear_screen(17);
}

void draw_white_screen()
{
	system_clear_screen(15);
}

void draw_black_screen()
{
	system_clear_screen(0);
}

void draw_pink_screen()
{
	system_clear_screen(13);
}

int quater_width = SCR_HEIGHT / 4;
int quater_height = SCR_HEIGHT / 4;
int last_row = SCR_HEIGHT * 4 / 5;
int last_line = SCR_HEIGHT * 4 / 5;
int line_width = 4;
int line_num = 4;
int row_width = 4;
int row_num = 4;

void draw_border(void)
{
	int i,j,k;
	for (i = 0;i < row_num;i++)
	{
		for (j = 0;j < row_width;j++)
		{
			for (k = 0;k < last_line;k++)
				vbuf[i * quater_height * SCR_WIDTH + j * SCR_WIDTH + k] = 1;
		}
	}
	for (i = 0;i < last_row;i++)
	{
		for (j = 0;j < line_num;j++)
		{
			for (k = 0;k < line_width;k++)
				vbuf[i * SCR_WIDTH + j * quater_width + k] = 1;
		}
	}

}

void draw_squares(int index,uint8_t color)
{
	int row = (index - 1) / 3;
	int line = index % 3;
	if (line == 0) line+=3;
	line -= 1;
	int start_place = row * (quater_height) * SCR_WIDTH + row_width * SCR_WIDTH + line_width + line * quater_width;
	int i,j;
	for (i = 0;i < quater_height - 4;i++)
	{
		for (j = start_place + i * SCR_WIDTH;j < start_place + i * SCR_WIDTH + quater_width - line_width;j++)
		vbuf[j] = color;
	}

}
