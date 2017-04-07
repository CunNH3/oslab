#include "../include/game.h"
#include "../include/common.h"
#include "../include/string.h"
#include "../include/adt/linklist.h"
#include "../include/device/video.h"
#include "../include/x86/x86.h"

void redraw_screen()
{
	fly_t it;
	const char *hit,*miss;
	prepare_buffer();
	for (it = characters();it != NULL;it = it->_next) 
	{
		static char buf[2];
		buf[0] = it->text + 'A';
		buf[1] = 0;
		int color = rand() % 11 + 10;
		draw_string(buf,it->x,it->y,color);
	}
	printk("test\n");
	draw_string(itoa(last_key_code()),SCR_HEIGHT - 8,0,48);
	hit = itoa(get_hit());
	draw_string(hit,0,SCR_WIDTH - strlen(hit) * 8,10);
	miss = itoa(get_miss());
	draw_string(miss,SCR_HEIGHT - 8,SCR_WIDTH - strlen(miss) * 8,12);
	draw_string(itoa(get_fps()),0,0,14);
	draw_string("FPS",0,strlen(itoa(get_fps())) * 8,14);
	display_buffer();
}

