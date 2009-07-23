#include "grub_multiboot.h"

class Multiboot
{
	private:
		unsigned long multiboot_magic;
		multiboot_info_t *multiboot_info;

	public:
		Multiboot(unsigned long magic, multiboot_info_t *mbi);
		
		void debug(Screen *Screen);
};
