BOOT    := boot.bin
KERNEL  := kernel.bin
GAME    := game.bin
PROGRAM := program.bin
IMAGE   := disk.bin

CC      := gcc
LD      := ld
OBJCOPY := objcopy
DD      := dd
QEMU    := qemu-system-i386
GDB     := gdb

CFLAGS := -Wall -Werror -Wfatal-errors -fno-stack-protector
CFLAGS += -MD
CFLAGS += -std=gnu11 -m32 -c
CFLAGS += -I ./include
CFLAGS += -O0 
CFLAGS += -fno-builtin
CFLAGS += -ggdb3

QEMU_OPTIONS := -serial stdio
QEMU_OPTIONS += -monitor telnet:127.0.0.1:1111,server,nowait

QEMU_DEBUG_OPTIONS := -S
QEMU_DEBUG_OPTIONS += -s

GDB_OPTIONS := -ex "target remote 127.0.0.1:1234"
GDB_OPTIONS += -ex "symbol $(KERNEL)"

OBJ_DIR        := obj
LIB_DIR        := lib
BOOT_DIR       := boot
KERNEL_DIR     := kernel
GAME_DIR	   := game
OBJ_LIB_DIR    := $(OBJ_DIR)/$(LIB_DIR)
OBJ_BOOT_DIR   := $(OBJ_DIR)/$(BOOT_DIR)
OBJ_KERNEL_DIR := $(OBJ_DIR)/$(KERNEL_DIR)
OBJ_GAME_DIR   := $(OBJ_DIR)/$(GAME_DIR)

KERNEL_LD_SCRIPT := $(shell find $(KERNEL_DIR) -name "*.ld")
GAME_LD_SCRIPT	 := $(shell find $(GAME_DIR) -name "*.ld")

LIB_C := $(wildcard $(LIB_DIR)/*.c)
LIB_O := $(LIB_C:%.c=$(OBJ_DIR)/%.o)

BOOT_S := $(wildcard $(BOOT_DIR)/*.S)
BOOT_C := $(wildcard $(BOOT_DIR)/*.c)
BOOT_O := $(BOOT_S:%.S=$(OBJ_DIR)/%.o)
BOOT_O += $(BOOT_C:%.c=$(OBJ_DIR)/%.o)

KERNEL_C := $(shell find $(KERNEL_DIR) -name "*.c")
KERNEL_S := $(shell find $(KERNEL_DIR) -name "*.S")
KERNEL_O := $(KERNEL_C:%.c=$(OBJ_DIR)/%.o)
KERNEL_O += $(KERNEL_S:%.S=$(OBJ_DIR)/%.o)

GAME_C := $(shell find $(GAME_DIR) -name "*.c")
GAME_O := $(GAME_C:%.c=$(OBJ_DIR)/%.o)

include config/Makefile.build
include config/Makefile.git

$(IMAGE): $(BOOT) $(PROGRAM)
	@$(DD) if=/dev/zero of=$(IMAGE) count=10000         > /dev/null
	@$(DD) if=$(BOOT) of=$(IMAGE) conv=notrunc          > /dev/null
	@$(DD) if=$(PROGRAM) of=$(IMAGE) seek=1 conv=notrunc > /dev/null

$(BOOT): $(BOOT_O)
	$(LD) -e start -Ttext=0x7C00 -m elf_i386 -nostdlib -o $@.out $^
	$(OBJCOPY) --strip-all --only-section=.text --output-target=binary $@.out $@
	@rm $@.out
	perl ./boot/genboot.pl $@
#	ruby ./boot/mbr.rb $@

$(OBJ_BOOT_DIR)/%.o: $(BOOT_DIR)/%.[cS]
	@mkdir -p $(OBJ_BOOT_DIR)
	$(CC) $(CFLAGS) -Os -I ./boot/include $< -o $@

#$(OBJ_BOOT_DIR)/%.o: $(BOOT_DIR)/%.c
#	@mkdir -p $(OBJ_BOOT_DIR)
#	$(CC) $(CFLAGS) -Os -I ./boot/inc $< -o $@

$(PROGRAM): $(KERNEL) $(GAME)
	cat $(KERNEL) $(GAME) > $(PROGRAM)

$(OBJ_LIB_DIR)/%.o : $(LIB_DIR)/%.c
	@mkdir -p $(OBJ_LIB_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(KERNEL): $(KERNEL_LD_SCRIPT)
$(KERNEL): $(KERNEL_O) $(LIB_O)
	$(LD) -m elf_i386 -T $(KERNEL_LD_SCRIPT) -nostdlib -o $@ $^ $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)
	perl ./kernel/genkern.pl $@

$(OBJ_KERNEL_DIR)/%.o: $(KERNEL_DIR)/%.[cS]
	mkdir -p $(OBJ_DIR)/$(dir $<)
	$(CC) $(CFLAGS) -I ./kernel/include $< -o $@

$(GAME): $(GAME_LD_SCRIPT)
$(GAME): $(GAME_O) $(LIB_O)
	$(LD) -m elf_i386 -T $(GAME_LD_SCRIPT) -nostdlib -o $@ $^ $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)
	$(call git_commit, "compile game", $(GITFLAGS))

$(OBJ_GAME_DIR)/%.o: $(GAME_DIR)/%.c
	mkdir -p $(OBJ_DIR)/$(dir $<)
	$(CC) $(CFLAGS) -I ./game/include $< -o $@

DEPS := $(shell find -name "*.d")
-include $(DEPS)

.PHONY: qemu debug gdb clean

qemu: $(IMAGE)
	$(QEMU) $(QEMU_OPTIONS) $(IMAGE)
	$(call git_commit, "run qemu", $(GITFLAGS))

# Faster, but not suitable for debugging
qemu-kvm: $(IMAGE)
	$(QEMU) $(QEMU_OPTIONS) --enable-kvm $(IMAGE)

debug: $(IMAGE)
	$(QEMU) $(QEMU_DEBUG_OPTIONS) $(QEMU_OPTIONS) $(IMAGE)
	$(call git_commit, "debug", $(GITFLAGS))

gdb:
	$(GDB) $(GDB_OPTIONS)
	$(call git_commit, "run gdb", $(GITFLAGS))

clean:
	@rm -rf $(OBJ_DIR) 2> /dev/null
	@rm -rf $(BOOT)    2> /dev/null
	@rm -rf $(KERNEL)  2> /dev/null
	@rm -rf $(GAME)	   2> /dev/null
	@rm -rf $(PROGRAM) 2> /dev/null
	@rm -rf $(IMAGE)   2> /dev/null

submit: clean
	cd .. && tar cvj $(shell pwd | grep -o '[^/]*$$') > $(STU_ID).tar.bz2

commit:
	@git commit --allow-empty

log:
	@git log --author=dancingflower
