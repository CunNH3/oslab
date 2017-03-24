#include "x86/x86.h"
#include "game.h"
#include "string.h"
#include "device/timer.h"

#define FPS 50
#define CHARACTER_PER_SECOND 5
#define UPDATE_PER_SECOND 100

volatile int tick = 0;

void timer_event(void)
{
	tick ++;
}

static int real_fps;

void set_fps(int value)
{
	real_fps = value;
}
int get_fps()
{
	return real_fps;
}

void main_loop(void)
{
	int now = 0, target;
	int num_draw = 0;
	bool redraw;
	while (TRUE)
	{
		wait_for_interrupt();
		disable_interrupt();
		if (now == tick)
		{
			enable_interrupt();
			continue;
		}
		
		assert(now < tick);
		target = tick;
		enable_interrupt();

		redraw = FALSE;
		while (update_keypress())
			;

		while (now < target) 
		{ 

			if (now % (HZ / CHARACTER_PER_SECOND) == 0)
				create_new_letter();
			if (now % (HZ / UPDATE_PER_SECOND) == 0)
				update_letter_pos();
			if (now % (HZ / FPS) == 0)
				redraw = TRUE;
			if (now % (HZ / 2) == 0)
			{
				int now_fps = num_draw * 2 + 1;
				if (now_fps > FPS) now_fps = FPS;
				set_fps(now_fps);
				num_draw = 0;
			}
			now ++;
		}
		if (redraw)
		{
			num_draw ++;
			redraw_screen();
		}
	}
}
