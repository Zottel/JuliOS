#include <types.h>
#include <screen.h>

#include "multiboot.h"

Multiboot::Multiboot(unsigned long magic, multiboot_info_t *mbi)
{
	multiboot_magic = magic;
	multiboot_info = mbi;
}
		
void Multiboot::debug(Screen *Screen)
{
	Screen->print_string("\nMultiboot debugging:\n");
	
	if (multiboot_magic != MULTIBOOT_BOOTLOADER_MAGIC)
	{
		Screen->printf("Invalid magic number!\n");
		return;
	}
	
	Screen->printf("*Header:\n  valid magic number: %h\n  flags: %b\n", multiboot_magic, multiboot_info->flags);
	
	if(multiboot_info->flags & 0x00000001)
		Screen->printf("*amount of memory:\n  %d megabytes (%d kilobytes)\n", (multiboot_info->mem_upper + multiboot_info->mem_lower) / 1024, multiboot_info->mem_upper + multiboot_info->mem_lower);
	
	if(multiboot_info->flags & 0x00000002)
		Screen->printf("*boot device:\n  0x%x\n", multiboot_info->boot_device);
	
	if(multiboot_info->flags & 0x00000004)
		Screen->printf("*cmdline:\n  %s\n", (const char *)(multiboot_info->cmdline));
	
	//TODO: Not tested - try it please!
	if(multiboot_info->flags & 0x00000008)
		Screen->printf("*modules:\n  count: 0x%x\n  addr: 0x%x\n", multiboot_info->mods_addr, multiboot_info->mods_addr);
	
	if(multiboot_info->flags & 0x00000020)
		Screen->printf("*elf:\n  num: 0x%x\n  size: 0x%x\n  addr: 0x%x\n  shndx: 0x%x\n", multiboot_info->u.elf_sec.num, multiboot_info->u.elf_sec.size, multiboot_info->u.elf_sec.addr, multiboot_info->u.elf_sec.shndx);
		
	if(multiboot_info->flags & 0x00000040)
	{
		Screen->print_string("*memory map:\n");
		
		memory_map_t *mmap;
		
		Screen->printf ("  mmap_addr = 0x%x, mmap_length = 0x%x\n", (unsigned) multiboot_info->mmap_addr, (unsigned) multiboot_info->mmap_length);
		
		for (mmap = (memory_map_t *) multiboot_info->mmap_addr;
		     (unsigned long) mmap < multiboot_info->mmap_addr + multiboot_info->mmap_length;
		     mmap = (memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size)))
		     
             Screen->printf("  size = 0x%x, base_addr = 0x%x%x, length = 0x%x%x, type = 0x%x\n",
                            (unsigned) mmap->size,
                            (unsigned) mmap->base_addr_high,
                            (unsigned) mmap->base_addr_low,
                            (unsigned) mmap->length_high,
                            (unsigned) mmap->length_low,
                            (unsigned) mmap->type);
	}
	
	
	if(multiboot_info->flags & 0x00000200)
	{
		Screen->printf("*Provided bootloader name:\n  %s\n", (const char *)(multiboot_info->boot_loader_name));
	}
}
