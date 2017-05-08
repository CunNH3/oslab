#include "include/common.h"
#include "include/x86.h"
#include "include/video.h"

bool keyboard_process();
void init_effect();
void game_loop();
void testprintf();

static bool restart;
void enable_restart()
{
	restart = true;
}
void close_restart()
{
	restart = false;
}


void game_main()
{
	testprintf();
	printf("Game Start!\n");
	//sti();
	restart = false;
	while(1)
	{
		//hlt();
		while(keyboard_process());
		if(restart)
		{
			clear_buffer();
			display_buffer();
			init_effect();
			game_loop();
		}
	}
}
