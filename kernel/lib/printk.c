#include "common.h"

/* implement this function to support printk */
void GetStr(char *str,int base,int n)
{
	uint32_t number;
	number = (uint32_t)n;
	char start[30];
	char *temp = start;
	if (!number)
	{
		*str = '0';
		*(str + 1) = '\0';
		return;
	}
	while (number != 0)
	{
		int modulo = number % base;
		if (modulo < 10) *temp = '0' + modulo; else *temp = 'a' + modulo - 10;
		number = number / base;
		temp++;
	}
	temp--;
	while (temp != start - 1)
	{
		*str = *temp;
		str++;
		temp--;
	}
	*str = '\0';
}

void vfprintf(void (*printer)(char), const char *ctl, void **args)
{
	char ch;
	for (ch = *ctl;*ctl != '\0';ctl++,ch = *ctl)
	{
		if ((ch == '%') && (*(ctl + 1) != '\0'))
		{
			ctl++;
			char str[150];
			if (*ctl == 'd')
			{
				GetStr(str,10,*(int*)args);
				char *cha;
				for (cha = str;*cha != '\0';cha++)
					printer(*cha);
				args++;
			}
			else
			if (*ctl == 'x')
			{
				GetStr(str,16,*(int*)args);
				char *cha;
				for (cha = str;*cha != '\0';cha++)
					printer(*cha);
				args++;
			}
			else
			if (*ctl == 's')
			{
				char *temp = *(char **)args;
				while (*temp != '\0')
				{
					printer(*temp);
					temp++;
				}
				args++;

			}
			else
			if (*ctl == 'c')
			{
				printer(*(char*)args);
				args++;
			}
			else
			{
				printer('%');
				args++;
			}
		}
			else {printer(ch);}
	}
}

extern void serial_printc(char);

/* __attribute__((__noinline__))  here is to disable inlining for this function to avoid some optimization problems for gcc 4.7 */
void __attribute__((__noinline__)) 
printk(const char *ctl, ...) {
	void **args = (void **)&ctl + 1;
	vfprintf(serial_printc, ctl, args);
}
