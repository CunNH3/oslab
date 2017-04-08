#include "../irq/irq.h"
#include "../include/syscall.h"
#include "../include/string.h"
#include "../include/stdio.h"

int do_syscall(TrapFrame *tf)
{
	return tf->eax;
}
