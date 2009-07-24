# Julios Makefile

# Configuration
VERSION := 1

CC := /usr/bin/gcc
CPLUSPLUSC := /usr/bin/g++
ASM := nasm

CFLAGS := -m32 -ffreestanding -Wall -Werror -nostdlib -nostartfiles -nodefaultlibs -I "$(CURDIR)" $(CFLAGS)
#CPLUSPLUSFLAGS := -ffreestanding -nostdlib -fno-builtin -fno-rtti -fno-exceptions
CPLUSPLUSFLAGS := -m32 -Wall -nostartfiles -nostdlib -fno-builtin -fno-rtti -fno-exceptions -I "$(CURDIR)" $(CPLUSPLUSFLAGS)
LDFLAGS := -melf_i386 -nostdlib $(LDFLAGS)

OBJFILES := startup.o kernel.o screen.o memory.o io.o interrupts.o keyboard.o multiboot.o gdt.o
OUTFILE := kernel

.PHONY: clean

# Rules
kernel: $(OBJFILES)
	@echo "Assembling kernel image ... "
	@ld $(LDFLAGS) -T ld.conf -o kernel $(OBJFILES)
	
%.o: %.asm Makefile
	@echo "Compiling $<... "
	@$(ASM) -f elf -o $@ $<;

%.o: %.c Makefile
	@echo "Compiling $<... "
	@$(CC) $(CFLAGS) -c $< -o $@;
	
%.o: %.cpp Makefile
	@echo "Compiling $<... "
	@$(CPLUSPLUSC) $(CPLUSPLUSFLAGS) -c $< -o $@;
	
all: kernel

clean:
	@rm *.o kernel
