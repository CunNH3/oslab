#include "include/common.h"
#include "include/x86.h"
#include "include/video.h"

bool process_keys();
void init_effect();
void game_loop();

static bool reborn;
void enable_reborn()
{
	reborn = true;
}
void close_reborn()
{
	reborn = false;
}


void game_main()
{
	printf("Game Start!\n");
	sti();
	reborn = false;
	while(1)
	{
		hlt();
		while(process_keys());
		if(reborn)
		{
			clear_buffer();
			display_buffer();
			init_effect();
			game_loop();
		}
	}
}
