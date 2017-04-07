#include "../irq/irq.h"

int do_syscall(TrapFrame *tf)
{
	return tf->eax;
}
