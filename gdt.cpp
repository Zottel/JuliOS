#include <types.h>
#include <screen.h>
#include <memory.h>

#include "gdt.h"

GDT::GDT(u16int size)
{
	entry = (struct gdt_entry *) malloc((1 + size) * sizeof(struct gdt_entry));
	entry = (struct gdt_entry *)(((u32int) entry) + (((u32int) entry) % 16));
	header = (struct gdt_header *) malloc(sizeof(struct gdt_header));
	
	gdt_size = size;
	header->limit = (size * sizeof(struct gdt_entry)) - 1;
	header->address = (u32int)entry;
	
	// System code segment:
	descriptor_setlimit(0x01, 0xFFFFF);
	descriptor_settype(0x01, 1);
	
	// System data segment:
	descriptor_setlimit(0x02, 0xFFFFF);
	descriptor_settype(0x02, 0);
}

void GDT::load()
{

	asm volatile ("xchg %bx, %bx");
	asm volatile ("lgdt %0" : : "m" (*header));
	asm volatile ("mov $0x10, %ax");
	asm volatile ("mov %ax, %ds");
	asm volatile ("mov %ax, %es");
	asm volatile ("mov %ax, %ss");
	asm volatile ("mov %ax, %fs");
	asm volatile ("mov %ax, %gs");
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
		entry[descriptor].access = 0x9A;
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
			((gdt_entry *) (header->address))[i].limit_low,
			((gdt_entry *) (header->address))[i].base_low,
			((gdt_entry *) (header->address))[i].base_middle,
			((gdt_entry *) (header->address))[i].access,
			((gdt_entry *) (header->address))[i].flags,
			((gdt_entry *) (header->address))[i].base_high,
			*((u16int *)&((gdt_entry *) (header->address))[i].limit_low),
			*((u16int *)&((gdt_entry *) (header->address))[i].base_low),
			*((u16int *)&((gdt_entry *) (header->address))[i].base_middle),
			*((u16int *)&((gdt_entry *) (header->address))[i].flags));
	}
}
