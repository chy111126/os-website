C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c programs/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h programs/*.h)
# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o} 

# Change this if your cross-compiler is somewhere else
CC = i386-elf-gcc
GDB = i386-elf-gdb
# -g: Use debugging symbols in gcc
CFLAGS = -g -ffreestanding -Wall -Wextra -fno-exceptions -m32

# First rule is run by default
# For os-image, pad the disk to standard disk sizes such that v86 can read it
pad-os-image: pre-pad-os-image
	dd if=/dev/zero of=../build/padded-os-image.bin bs=1 count=$(shell expr $(shell echo 163840) - $(shell ls -l ../build/os-image.bin | cut -d" "  -f8)) seek=$(shell ls -l ../build/os-image.bin | cut -d" "  -f8)

pre-pad-os-image: ../build/os-image.bin
	cp ../build/os-image.bin ../build/padded-os-image.bin

../build/os-image.bin: boot/bootsect.bin kernel.bin
	cat $^ > ../build/os-image.bin

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

# Used for debugging purposes
kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ 

run: pad-os-image
	qemu-system-i386 -fda ../build/os-image.bin

# Open the connection to qemu and load our kernel-object file with symbols
debug: os-image.bin kernel.elf
	qemu-system-i386 -s -fda os-image.bin -d guest_errors,int &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# Generic rules for wildcards
# To make an object, always compile from its .c
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf ../build/*.bin
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o libc/*.o