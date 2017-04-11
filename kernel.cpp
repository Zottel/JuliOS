#define DEBUG 1

#include <types.h>
#include <screen.h>
#include <interrupts.h>
#include <memory.h>
#include <keyboard.h>
#include <multiboot.h>
#include <gdt.h>

// Prevent name mangling for interfacing with ASM.
extern "C" int kmain(unsigned long magic, multiboot_info_t *mbi);

int kmain(unsigned long magic, multiboot_info_t *mbi)
{
	Screen Title((char *) 0xb8000, 80, 1);
	Title.setcolor(0x4E);
	Title.clear();
	Title.printf("         JuliOS - Lame Algorithms paired with incredibly stupid design.");
	
	// Create a Screen object assigned to the virtual memory.
	Screen Screen((char *) (0xb8000 + (0x50 * 2)), 80, 25);
	
	// Set the color to yellow on dark grey and clear the screen.
	Screen.setcolor(0xE4);
	Screen.clear();
	
	// Now we're ready to display our first message.
	//Screen.print_string("JuliOS startet!\n");
	
	// I LOVE Multiboot!
	//Screen.print_string("\nErstelle Multiboot Objekt.\n");
	//Multiboot MBoot(magic, mbi);
	//MBoot.debug(&Screen);
	
	// Create a memory manager - stored on the stack for now.
	Memory mem;
	// Register a memory pool to allocate from.
	mem.register_pool((void *)0x200000, 0x100000);
	// Have the chicken hatch from it's own egg.
	Memory *memory = (Memory *) mem.allocopy(((void *) &mem), sizeof(Memory));
	// And have the copy run the show.
	set_memory_handler(memory);
	// Allocate some space.
	//Screen.printf("Allocation of a 0x10000 bytes large chunk was %s\n", malloc(0x10000) ? "successful." : "not successful!");
	// Proudly present the working result :)
	//memory->debug(&Screen);
	
	// Produce a triple fault (for comparison to those we don't want :)
	//asm volatile ("int $0x01");
	
	GDT *gdt = new GDT();
	gdt->addCodeSegment (0, 0xFFFFF, 0, 0, 0);
	gdt->addDataSegment (0, 0xFFFFF, 1, 0, 0, 0);
	
	//gdt->debug(&Screen);
	
	
	return 0;
}
