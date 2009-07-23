#include <interrupts.h>
#include <io.h>

void Interrupts::setup()
{
	Interrupts::remap();
	//Interrupts::idt_setup();
	//Interrupts::enable();
}

void Interrupts::remap()
{
	IO::outb(0x20, 0x11);
	IO::outb(0xA0, 0x11);
	IO::outb(0x21, 0x20);
	IO::outb(0xA1, 0x28);
	IO::outb(0x21, 0x04);
	IO::outb(0xA1, 0x02);
	IO::outb(0x21, 0x01);
	IO::outb(0xA1, 0x01);
	IO::outb(0x21, 0x0);
	IO::outb(0xA1, 0x0);
}

void Interrupts::enable()
{
	__asm__("sti");
}

void Interrupts::disable()
{
	__asm__("cli");
}
