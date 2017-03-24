CC      = gcc
LD      = ld
CFLAGS  = -Wno-main -m32 -static -ggdb -MD -Wall -Werror -I./include -O0 \
		 -fno-builtin -fno-stack-protector -fno-omit-frame-pointer
ASFLAGS = -m32 -MD -I./include
LDFLAGS = -melf_i386
QEMU = qemu-system-i386

include config/Makefile.git
include config/Makefile.build

CFILES = $(shell find src/ -name "*.c")
SFILES = $(shell find src/ -name "*.S")
OBJS = $(CFILES:.c=.o) $(SFILES:.S=.o)

game.img: game
	@cd boot; make
	cat boot/bootblock game > game.img

game: $(OBJS)
	$(call git_commit, "compile game", $(GITFLAGS))
	$(LD) $(LDFLAGS) -e game_init -Ttext 0x00100000 -o game $(OBJS)

-include $(patsubst %.o, %.d, $(OBJS))


.PHONY: clean debug qemu submit commit log

qemu: game.img
	$(call git_commit, "qemu", $(GITFLAGS))
	$(QEMU) -serial stdio game.img
debug: game.img
	$(call git_commit, "debug", $(GITFLAGS))
	$(QEMU) -serial stdio -s -S game.img

clean:
	@cd boot; make clean
	rm -f game game.img $(OBJS) $(OBJS:.o=.d)
	-rm -rf obj 2> /dev/null
	-rm -f *log.txt entry $(FLOAT) 2> /dev/null

submit: clean
	$(call git_commit, "submit", $(GITFLAGS))
	cd .. && tar cvj $(shell pwd | grep -o '[^/]*$$') > $(STU_ID).tar.bz2

commit:
	@git commit --allow-empty

log:
	@git log --author=dancingflower
