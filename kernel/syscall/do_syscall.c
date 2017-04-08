#include "../include/irq.h"
//#include "../include/syscall.h"
//#include "../include/string.h"
//#include "../include/stdio.h"
#include "../include/common.h"

enum {SYS_time, SYS_keyboard, SYS_video};

extern uint32_t time_tick;
int handle_keys();
int load_vmem(uint8_t*);

void do_syscall(TrapFrame *tf)
{
	switch(tf->eax)
	{
		case SYS_time: tf->eax = time_tick; break;
		case SYS_keyboard: tf->eax = handle_keys(); break;
		case SYS_video: tf->eax = load_vmem((uint8_t *)tf->ebx); break;
		default: panic("Unhandled system call: id = %d", tf->eax);
	}
}
