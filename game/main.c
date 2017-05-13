#include "include/common.h"
#include "include/x86.h"
#include "include/video.h"
#include "include/string.h"
#include "include/game.h"
#include "include/keyboard.h"
#include "include/timer.h"
#include "include/system.h"

volatile int tick = 0;

void timer_event(void)
{
	tick++;
}

void init_game()
{
	syscall(0, timer_event);
	syscall(1, keyboard_event);
}

void testprintf();

void sleep(int time)
{
	int pretime = tick;
	while (tick - pretime < time * 100);
}
int game_main()
{
	init_game();
	testprintf();
	while (1)
	{
		printf("Game init!\n");
		clear_letter_pressed();
		black_screen();
		prepare_buffer();
		draw_border();
		display();
		init_squares();
		printf("Game start!\n");
		game_loop();
		sleep(2);
	}
	return 0;
}
