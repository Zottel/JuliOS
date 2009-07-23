#include <io.h>

void IO::wait(void)
{
	outb(0x80, 0);
	//asm volatile("outb %%al, $0x80" : : "a"(0));
}

unsigned char IO::inb(unsigned short port)
{
	unsigned char ret;
	asm volatile ("inb %1,%0":"=a"(ret):"Nd"(port));
	return ret;
}

void IO::outb(unsigned short port, unsigned char val)
{
	asm volatile("outb %0,%1"::"a"(val), "Nd" (port));
}
