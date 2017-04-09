#include "../include/common.h"
#include "../include/syscall.h"
#include "../include/stdio.h"

//int	vsnprintf(char, int , const char *, va_list);
int __attribute__((__noinline__))
printf(const char *ctl, ...)
{
	static char buf[256];
	void *args = (void **)&ctl + 1;
	int len = vsnprintf(buf, 256, ctl, args);
	return write(1, buf, len);
}
