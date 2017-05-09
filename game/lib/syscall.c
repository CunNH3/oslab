#include "../include/common.h"

enum {SYS_time, SYS_keyboard,SYS_write, SYS_video};

static inline int //__attribute__((__noinline__))
syscall(int id, ...)
{
	int ret;
	int *args = &id;
	asm volatile("int $0x80": "=a"(ret) : "a"(args[0]), "b"(args[1]), "c"(args[2]), "d"(args[3]));
	return ret;
}

int get_time()
{
	return syscall(SYS_time);
}

int get_keyboard()
{
	return syscall(SYS_keyboard);
}

int write(int fd, char *buf,int len)
{
	return syscall(SYS_write,fd,buf,len);
}

int display_video(uint8_t *buffer)
{
	return syscall(SYS_video, buffer);
} 
