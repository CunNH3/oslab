CC      := gcc
LD      := ld
QEMU    := qemu-system-i386
GDB     := gdb

OBJ_DIR        := obj
LIB_DIR        := lib
BOOT_DIR       := boot
KERNEL_DIR     := kernel
GAME_DIR	   := game
OBJ_LIB_DIR    := $(OBJ_DIR)/$(LIB_DIR)
OBJ_BOOT_DIR   := $(OBJ_DIR)/$(BOOT_DIR)
OBJ_KERNEL_DIR := $(OBJ_DIR)/$(KERNEL_DIR)
OBJ_GAME_DIR := $(OBJ_DIR)/$(GAME_DIR)

LIB_C := $(shell find $(LIB_DIR) -name "*.c")
LIB_O := $(LIB_C:%.c=$(OBJ_DIR)/%.o)

GAME_C := $(shell find $(GAME_DIR) -name "*.c")
GAME_O := $(GAME_C:%.c=$(OBJ_DIR)/%.o)

KERNEL_C := $(shell find $(KERNEL_DIR) -name "*.c")
KERNEL_S := $(shell find $(KERNEL_DIR) -name "*.S")
KERNEL_O := $(KERNEL_C:%.c=$(OBJ_DIR)/%.o) 
KERNEL_O += $(KERNEL_S:%.S=$(OBJ_DIR)/%.o)


CFLAGS  = -Wno-main -m32 -static -ggdb3 -MD -Wall -Werror -I./include -O0 \
		 -fno-builtin -fno-stack-protector -fno-omit-frame-pointer
ASFLAGS = -m32 -MD
LDFLAGS = -melf_i386
QEMU    := qemu-system-i386

include config/Makefile.git
include config/Makefile.build

include boot/Makefile.part

CFILES = $(shell find src/ -name "*.c")
SFILES = $(shell find src/ -name "*.S")
OBJS 	:= $(LIB_O) $(GAME_O) $(KERNEL_O)

game.img: game bootblock
	cat obj/boot/bootblock obj/game/game > obj/game.img

game: $(OBJS)
	@mkdir -p obj/game
	$(call git_commit, "compile game", $(GITFLAGS))
	$(LD) $(LDFLAGS) -e game_main -Ttext 0x00100000 -o obj/game/game $(OBJS)

$(OBJ_LIB_DIR)/%.o : $(LIB_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/$(dir $<)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_GAME_DIR)/%.o : $(GAME_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/$(dir $<)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_KERNEL_DIR)/%.o : $(KERNEL_DIR)/%.[cS]
	@mkdir -p $(OBJ_DIR)/$(dir $<)
	$(CC) $(CFLAGS) $< -o $@

#-include $(patsubst %.o, %.d, $(OBJS))
IMAGES	:= $(OBJ_DIR)/game.img
GDBPORT := $(shell expr `id -u` % 5000 + 25000)
QEMUOPTS = $(OBJ_DIR)/os.img -serial mon:stdio
QEMUOPTS += $(shell if $(QEMU) -nographic -help | grep -q '^-D '; then echo '-D qemu.log'; fi)

.PHONY: clean debug gdb display submit commit log

display:
	@echo $(OBJS)

.gdbinit: .gdbinit.tmpl
	sed "s/localhost:1234/localhost:$(GDBPORT)/" < $^ > $@
pre-qemu: .gdbinit

gdb:
	gdb -x .gdbinit

qemu: game.img pre-qemu
	$(QEMU) $(QEMUOPTS)
	$(call git_commit, "run qemu", $(GITFLAGS))

debug: $(IMAGES) pre-qemu
	$(QEMU) -s $(QEMUOPTS) -S 

clean: clean-mdr
	rm -f obj/game/game obj/game.img $(OBJS) $(OBJS:.o=.d)

submit: clean
	cd .. && tar cvj $(shell pwd | grep -o '[^/]*$$') > $(STU_ID).tar.bz2

commit:
	@git commit --allow-empty

log:
	@git log --author=dancingflower
