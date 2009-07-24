#include <types.h>
#include <screen.h>
#include <memory.h>

#include "gdt.h"

GDT::GDT()
{
	used_entries = 1;
}

void GDT::debug(Screen *Screen)
{
	for (int i = 0; i < used_entries; i++) {
		struct gdt_entry entry = entries[i];
		u32int *p = (u32int *) &entry;
		Screen->printf("Together: 0x%x 0x%x\n", *p, *(p + 1));
	}
}

void GDT::addSegment (u32int base, u32int limit, u8int present, u8int system, u8int type, u8int privLevel, u8int granularity)
{
	gdt_entry *entry = &entries[used_entries++];

	entry->limit_low = limit & 0xFFFF;
	entry->base_low = base & 0xFFFF;
	entry->base_middle = (base >> 16) & 0xFF;
	entry->base_high = (base >> 24) & 0xFF;
	entry->access = ((present ? 1 : 0) << 7) | (privLevel << 5) | ((system ? 0 : 1) << 4) | (type & 0x0F);
	entry->granularity = 0xC0 | (limit >> 16);
}

void GDT::addDataSegment (u32int base, u32int limit, u8int readWrite, u8int expandDown, u8int privLevel, u8int byteGranularity)
{
	addSegment (base, limit, 1, 0, (readWrite ? 1 : 0) << 1 | (expandDown ? 1 : 0) << 2, privLevel, !byteGranularity);
}

void GDT::addCodeSegment (u32int base, u32int limit, u8int readExecute, u8int privLevel, u8int byteGranularity)
{
	addSegment (base, limit, 1, 0, 0x8 | (readExecute ? 1 : 0) << 1, privLevel, !byteGranularity);
}

void GDT::install ()
{
	struct gdt_header header;

	header.address = (u32int) &entries;
	header.limit = (used_entries * sizeof (struct gdt_entry)) - 1;

	asm volatile ("lgdt %0" : : "m" (header));
	asm volatile ("mov $0x10, %ax");
	asm volatile ("mov %ax, %ds");
	asm volatile ("mov %ax, %ss");
	asm volatile ("ljmp $0x08, $next");
	asm volatile ("next: ");
}
