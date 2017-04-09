#include "../include/types.h"
#include "../include/stdio.h"
#include "../include/string.h"
#include "../include/stdarg.h"
#include "../include/error.h"

static const char * const error_string[MAXERROR] =
{
	[E_UNSPECIFIED]	= "unspecified error",
	[E_BAD_ENV]	= "bad environment",
	[E_INVAL]	= "invalid parameter",
	[E_NO_MEM]	= "out of memory",
	[E_NO_FREE_ENV]	= "out of environments",
	[E_FAULT]	= "segmentation fault",
};

static void print_number(void (*putch)(int, void*), void *putdat, unsigned long long num, unsigned base, int width, int padc)
{
	if (num >= base)
		print_number(putch, putdat, num / base, base, width - 1, padc);
	else
		while (--width > 0) putch(padc, putdat);
	putch("0123456789abcdef"[num % base], putdat);
}

static unsigned long long get_unsigned(va_list *ap, int lflag)
{
	if (lflag >= 2)
		return va_arg(*ap, unsigned long long);
	else 
	if (lflag)
		return va_arg(*ap, unsigned long);
	else
		return va_arg(*ap, unsigned int);
}

static long long get_integer(va_list *ap, int lflag)
{
	if (lflag >= 2)
		return va_arg(*ap, long long);
	else 
	if (lflag)
		return va_arg(*ap, long);
	else
		return va_arg(*ap, int);
}

void print_format(void (*putch)(int, void*), void *putdat, const char *fmt, ...);

void vprint_format(void (*putch)(int, void*), void *putdat, const char *fmt, va_list ap)
{
	register const char *p;
	register int ch, err;
	unsigned long long num;
	int base, lflag, width, pre, altflag;
	char padc;

	while (1) {
		while ((ch = *(unsigned char *) fmt++) != '%')
		{
			if (ch == '\0') return;
			putch(ch, putdat);
		}

		padc = ' ';
		width = -1;
		pre = -1;
		lflag = 0;
		altflag = 0;
	reset_switch:
		switch (ch = *(unsigned char *) fmt++) 
		{
			case '-':
				padc = '-';
				goto reset_switch;

			case '0':
				padc = '0';
				goto reset_switch;

			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				for (pre = 0; ; ++fmt) 
				{
					pre = pre * 10 + ch - '0';
					ch = *fmt;
					if (ch < '0' || ch > '9') break;
				}
				goto process_pre;

			case '*':
				pre = va_arg(ap, int);
				goto process_pre;

			case '.':
				if (width < 0) width = 0;
				goto reset_switch;

			case '#':
				altflag = 1;
				goto reset_switch;

			process_pre:
				if (width < 0) width = pre, pre = -1;
				goto reset_switch;

			case 'l':
				lflag++;
				goto reset_switch;

			case 'c':
				putch(va_arg(ap, int), putdat);
				break;

			case 'e':
				err = va_arg(ap, int);
				if (err < 0) err = -err;
				if (err >= MAXERROR || (p = error_string[err]) == NULL)
					print_format(putch, putdat, "error %d", err);
				else
					print_format(putch, putdat, "%s", p);
				break;

			case 's':
				if ((p = va_arg(ap, char *)) == NULL) p = "(null)";
				if (width > 0 && padc != '-')
					for (width -= strnlen(p, pre); width > 0; width--)
						putch(padc, putdat);
				for (; (ch = *p++) != '\0' && (pre < 0 || --pre >= 0); width--)
					if (altflag && (ch < ' ' || ch > '~'))
						putch('?', putdat);
					else
						putch(ch, putdat);
				for (; width > 0; width--)
					putch(' ', putdat);
				break;


			case 'd':
				num = get_integer(&ap, lflag);
				if ((long long) num < 0)
				{
					putch('-', putdat);
					num = -(long long) num;
				}
				base = 10;
				goto number;

			case 'u':
				num = get_unsigned(&ap, lflag);
				base = 10;
				goto number;

			case 'o':
				num = get_unsigned(&ap, lflag);
				base = 8;
				goto number;

			case 'p':
				putch('0', putdat);
				putch('x', putdat);
				num = (unsigned long long)
				(uintptr_t) va_arg(ap, void *);
				base = 16;
				goto number;

			case 'x':
				num = get_unsigned(&ap, lflag);
				base = 16;
			number:
				print_number(putch, putdat, num, base, width, padc);
				break;

			case '%':
				putch(ch, putdat);
				break;

			default:
				putch('%', putdat);
				for (fmt--; fmt[-1] != '%'; fmt--)
				/* do nothing */;
				break;
		}
	}
}

void print_format(void (*putch)(int, void*), void *putdat, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vprint_format(putch, putdat, fmt, ap);
	va_end(ap);
}

struct sprintbuf
{
	char *buf;
	char *ebuf;
	int cnt;
};

static void sprintputch(int ch, struct sprintbuf *b)
{
	b->cnt++;
	if (b->buf < b->ebuf) *b->buf++ = ch;
}

int vsnprintf(char *buf, int n, const char *fmt, va_list ap)
{
	struct sprintbuf b = {buf, buf + n - 1, 0};
	if (buf == NULL || n < 1)
		return -E_INVAL;
	vprint_format((void*)sprintputch, &b, fmt, ap);
	*b.buf = '\0';
	return b.cnt;
}

int snprintf(char *buf, int n, const char *fmt, ...)
{
	va_list ap;
	int rc;
	va_start(ap, fmt);
	rc = vsnprintf(buf, n, fmt, ap);
	va_end(ap);
	return rc;
}
