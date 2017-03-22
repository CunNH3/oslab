bootblock: boot.S main.c asm.h boot.h
	gcc -c -m32 boot.S
	gcc -c -m32 -O1 main.c
	ld -melf_i386 -e boot -Ttext 0x7C00 boot.o main.o -o bootblock.o
	@objcopy -S -O binary -j .text bootblock.o bootblock
	@perl ./genboot.pl bootblock

clean:
	rm -rf bootblock *.o
