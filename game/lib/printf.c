#include "../include/x86.h"
#include "../include/stdarg.h"
#include "../include/stdio.h"
#include "../include/system.h"

static void putch_f(int ch,int *cnt)
{
	system_serial_print(ch);
	(*cnt)++;
}

int vprintf(const char *fmt,va_list ap)
{
	int cnt = 0;
	vprintfmt((void*)putch_f,&cnt,fmt,ap);
	return cnt;
}

int printf(const char *fmt,...)
{
	va_list ap;
	int cnt;
	va_start(ap,fmt);
	cnt = vprintf(fmt,ap);
	va_end(ap);
	return cnt;
}
