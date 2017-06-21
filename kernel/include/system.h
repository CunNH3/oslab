#include"types.h"
#include"env.h"
#define  drawpixeloff (62)
#define  drawpixel (60)
#define  serialprint (61)
#define  clearscreen (63)
#define  initserial (64)
#define  inittimer (65)
#define  enableinterrupt (66)
#define  disenableinterrupt (67)
#define  env_fork (68)
#define  env_sleep (69)
#define  env_exit (70)
#define  create_thread (71)
#define  seminit (72)
#define  semopen (73)
#define  semdestroy (74)
#define  semwait (75)
#define  sempost (76)
#define  f_open (77)
#define  f_read (78)
#define  f_write (79)
#define  f_lseek (80)
#define  f_close (81)
#define  f_rewind (82)

int __attribute__((__noinline__)) syscall(int id,...);
void system_draw_pixel( int, int, int);
void system_serial_print(char);
void system_draw_pixel_off( int, int);
void system_clear_screen(int);
void system_init_serial();
void system_init_timer();
void system_enable_interrupt();
void system_disenable_interrupt();
int system_env_fork();
void system_env_sleep(uint32_t);
void system_env_exit();
struct Env*seek_next_runnable();
void kernel_timer_event();
int thread_create(void *);
void sem_init();
void sem_open(int,bool,int);
void sem_destroy(int);
int sem_wait(int);
void sem_post(int);
int file_open(const char *filename,int flag);
int file_read(int fd,void* buf,int len);
int file_write(int fd,void* buf,int len);
int file_lseek(int fd,int offset,int whence);
int file_close(int fd);
void file_rewind(int fd);
