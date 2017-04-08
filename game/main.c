#include "include/game.h"
#include "include/irq.h"
#include "include/x86/x86.h"
#include "include/timer.h"
#include "include/palette.h"
#include "include/assert.h"

//void testprintk();


void game_init(void)
{
/*	init_serial();
	init_timer();
	init_idt();
	init_intr();

	set_timer_intr_handler(timer_event);
	set_keyboard_intr_handler(keyboard_event);*/
	printk("game start!\n");
	//testprintk();
	enable_interrupt();
	main_loop();
	//printk("game test!\n");
	assert(0);
}
