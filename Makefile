# GNU make手册：http://www.gnu.org/software/make/manual/make.html
# ************ 遇到不明白的地方请google以及阅读手册 *************

# 编译器设定和编译选项
CC      = gcc
LD      = ld
CFLAGS  = -Wno-main -m32 -static -ggdb -MD -Wall -Werror -I./include -O0 \
		 -fno-builtin -fno-stack-protector -fno-omit-frame-pointer
ASFLAGS = -m32 -MD -I./include
LDFLAGS = -melf_i386
QEMU = qemu-system-i386

include config/Makefile.git
include config/Makefile.build
# 编译目标：src目录下的所有.c和.S文件
CFILES = $(shell find kernel/ -name "*.c")
SFILES = $(shell find kernel/ -name "*.S")
OBJS = $(CFILES:.c=.o) $(SFILES:.S=.o)

game.img: game
	@cd boot; make
	cat boot/bootblock game > game.img

game: $(OBJS)
	$(call git_commit, "game", $(GITFLAGS))
	$(LD) $(LDFLAGS) -e game_init -Ttext 0x00100000 -o game $(OBJS)

-include $(patsubst %.o, %.d, $(OBJS))

# 定义的一些伪目标
.PHONY: clean debug qemu submit commit log
# 如果make play运行失败，请检查qemu的安装
# 也可以修改这里的脚本，用其他类型的模拟器启动game.img
qemu: game.img
	$(call git_commit, "qemu", $(GITFLAGS))
	$(QEMU) -serial stdio game.img
debug: game.img
	$(call git_commit, "debug", $(GITFLAGS))
	$(QEMU) -serial stdio -s -S game.img

# make clean可以清除已生成的文件
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
