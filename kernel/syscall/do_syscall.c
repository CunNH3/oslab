#include "../irq/irq.h"
#include "../include/syscall.h"
#include "../include/string.h"
#include "../include/stdio.h"

void serial_printc(char);


static void sys_write(TrapFrame *tf)
{
	int i;
	for(i = 0;i < tf->edx;i++)
		serial_printc(*(char *)(tf->ecx + i));
	tf->eax = tf->edx;
}

bool query_key(int);
static void sys_keyboard(TrapFrame *tf)
{
	int i;
	int *keybd = (int *)tf->ebx;
	for (i = 0;i < tf->ecx;i++)
		if (keybd[i] == 0 && query_key(i)) keybd[i] = 1;
		else if (keybd[i] && !query_key(i)) keybd[i] = 0;
}
int Get_time();
void Delay(int);

void do_syscall(TrapFrame *tf)
{
	switch (tf->eax)
	{
		case SYS_write: sys_write(tf); break;
		case SYS_keyboard: sys_keyboard(tf); break;
		case SYS_time: tf->eax = Get_time(); break;
		case SYS_sleep: Delay(tf->ebx); break;
		default: printk("Unhandled system call: id = %d\n", tf->eax);
	}
}
