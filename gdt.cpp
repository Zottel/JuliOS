#include <types.h>
#include <screen.h>
#include <memory.h>

#include "gdt.h"

GDT::GDT(u16int size)
{
	entry = (struct gdt_entry *) malloc(size * sizeof(struct gdt_entry));
	header = (struct gdt_header *) malloc(size * sizeof(struct gdt_header));
	
	gdt_size = size;
	header->limit = (size * sizeof(struct gdt_entry));
	header->address = (u32int)entry;
}

void GDT::load(u16int code_descriptor)
{
/*
mov $0x10, %ax
mov %ax, %ds
mov %ax, %es
mov %ax, %ss
ljmp $0x8, $.1
.1:
*/

	asm volatile ("xchg %bx, %bx");
	asm volatile ("lgdt %0" : : "m" (header));
	asm volatile ("mov $0x10, %ax");
	asm volatile ("mov %ax, %ds");
	asm volatile ("mov %ax, %es");
	asm volatile ("mov %ax, %ss");
	asm volatile ("ljmp $0x8, $.newsegment");
	asm volatile (".newsegment:");
	//asm volatile ("hlt");
}

void GDT::descriptor_setbase(u16int descriptor, u32int base)
{
	entry[descriptor].base_low = base & 0xFFFF;
	entry[descriptor].base_middle = (base >> 16) & 0xFF;
	entry[descriptor].base_high = (base >> 24) & 0xFF;
}

void GDT::descriptor_setlimit(u16int descriptor, u32int limit)
{
	entry[descriptor].limit_low = limit & 0xFFFF;
	entry[descriptor].flags = ((limit >> 16) & 0x0F) | (entry[descriptor].flags & 0xF0);
}

void GDT::descriptor_settype(u16int descriptor, u8int code)
{
	if(code)
	{
		entry[descriptor].access = 0x9B;
		entry[descriptor].flags = 0xC0 | (entry[descriptor].flags & 0x0F);
	}
	else
	{
		entry[descriptor].access = 0x92;
		entry[descriptor].flags = 0xC0 | (entry[descriptor].flags & 0x0F);
	}
}

void GDT::debug(Screen *Screen)
{
	u16int i;
	Screen->printf("\nGDT: (limit: 0x%x, address: 0x%x)\n", header->limit, header->address);
	for(i = 0; i < gdt_size; i++)
	{
		Screen->printf("  limit_low=0x%x, base_low=0x%x, base_middle=0x%x, access=0x%x, flags=0x%x, base_high=0x%x\n  0x%x\n  0x%x\n  0x%x\n  0x%x\n",
			entry[i].limit_low,
			entry[i].base_low,
			entry[i].base_middle,
			entry[i].access,
			entry[i].flags,
			entry[i].base_high,
			*((u16int *)&entry[i].limit_low),
			*((u16int *)&entry[i].base_low),
			*((u16int *)&entry[i].base_middle),
			*((u16int *)&entry[i].flags));
	}
}
