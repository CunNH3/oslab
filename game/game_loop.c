#include "include/common.h"
#include "include/x86.h"
#include "include/keyboard.h"
#include "include/video.h"
#include "include/syscall.h"

uint32_t time_tick;

bool produce_enemy();
bool update_enemy();
bool produce_bullet();
bool update_bullet();
void update_player();
void draw_screen();

void game_loop()
{
	bool dead_flag = false;
	uint32_t now = get_time();
	uint32_t target;
	while(1)
	{
		//hlt();
		//cli();
		if(now == get_time())
		{
			//sti();
			continue;
		}

		target = get_time();
		//sti();

		while(keyboard_process());

		while(now < target)
		{
			if(now % 5 == 0) update_bullet();
			if(now % 8 == 0) update_player();
			if(now % 10 == 6) dead_flag = update_enemy();
			if(now % 10 == 0) draw_screen();
			if(now % 100 == 0) produce_bullet();
			if(now % 500 == 0) produce_enemy();
			now++;
		}
		if(dead_flag) break;
	}

}
