#include "../include/x86.h"

#define SERIAL_PORT  0x3F8

void init_serial(void)
{
	outb(SERIAL_PORT + 1, 0x00);
	outb(SERIAL_PORT + 3, 0x80);
	outb(SERIAL_PORT + 0, 0x01);
	outb(SERIAL_PORT + 1, 0x00);
	outb(SERIAL_PORT + 3, 0x03);
	outb(SERIAL_PORT + 2, 0xC7);
	outb(SERIAL_PORT + 4, 0x0B);
}

static inline int serial_idle(void)
{
	return (inb(SERIAL_PORT + 5) & 0x20) != 0;
}

void serial_printc(char ch)
{
	while (serial_idle() != true);
	outb(SERIAL_PORT, ch);
}

void serial_output_test()
{
	outb(SERIAL_PORT + 0, 'A');
	outb(SERIAL_PORT + 0, 'B');
	outb(SERIAL_PORT + 0, 'C');
	outb(SERIAL_PORT + 0, 'D');
	outb(SERIAL_PORT + 0, 'E');
	outb(SERIAL_PORT + 0, 'F');
	outb(SERIAL_PORT + 0, 'G');
}
