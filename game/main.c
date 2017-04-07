#include "../include/game.h"
#include "../include/irq.h"
#include "../include/x86/x86.h"
#include "../include/device/timer.h"
#include "../include/device/palette.h"
#include "../include/assert.h"

void testprintk();
void init_serial();
void init_timer();
void init_idt();
void init_intr();
void set_timer_intr_handler(void (*ptr)(void));
void set_keyboard_intr_handler(void (*ptr)(int));

void game_init(void)
{
	init_serial();
	init_timer();
	init_idt();
	init_intr();

	set_timer_intr_handler(timer_event);
	set_keyboard_intr_handler(keyboard_event);

	printk("game start!\n");
	testprintk();
	asm volatile("sti");
	enable_interrupt();
	main_loop();
	//printk("game test!\n");
	assert(0);
}
