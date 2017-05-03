#include "../include/irq.h"
#include "../include/common.h"
#include "../include/device/keyboard.h"

enum {SYS_time, SYS_keyboard, SYS_write, SYS_video};

extern uint32_t time_tick;

int pro_keyboard();
int fs_write(int, void *, int);
int display(uint8_t *);

void do_syscall(TrapFrame *tf)
{
	switch(tf->eax)
	{
		case SYS_time: 
			tf->eax = time_tick; 
			break;
		case SYS_keyboard: 
			tf->eax = pro_keyboard(); 
			break;
		case SYS_write: 
			tf->eax = fs_write(tf->ebx, (void *)tf->ecx, tf->edx); 
			break;
		case SYS_video: 
			tf->eax = display((uint8_t *)tf->ebx); 
			break;
		default: panic("Unhandled system call: id = %d", tf->eax);
	}
}
