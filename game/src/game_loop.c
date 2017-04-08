#include "../include/common.h"
#include "../include/x86.h"
#include "../include/keyboard.h"
#include "../include/video.h"

uint32_t time_tick;

bool generate_enemy();
bool update_enemy();
bool generate_bullet();
bool update_bullet();
void update_player();
void draw_screen();

void game_loop()
{
	bool dead_flag = false;
	uint32_t now = time_tick, target;
	while(1)
	{
		hlt();
		cli();
		if(now == time_tick)
		{
			sti();
			continue;
		}

		target = time_tick;
		sti();

		process_keys();

		while(now < target)
		{
			if(now % 5 == 0) update_bullet();
			if(now % 8 == 0) update_player();
			if(now % 10 == 6) dead_flag = update_enemy();
			if(now % 10 == 0) draw_screen();
			if(now % 100 == 0) generate_bullet();
			if(now % 500 == 0) generate_enemy();
			now++;
		}
		if(dead_flag) break;
	}

}
