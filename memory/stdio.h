#ifndef __STDIO_H__
#define __STDIO_H__

#include "../include/types.h"
#include "../include/stdarg.h"

void serial_printc(char);

void vprintfmt(const char *fmt, va_list ap);
void printfmt(const char *fmt, ...);

int printk(const char *, ...);

#endif
