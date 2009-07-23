#include <keyboard.h>
#include <io.h>

unsigned char Keyboard::status()
{
	return IO::inb(0x64);
}

int Keyboard::has_input()
{
	return (status() & Status_Input_Buffer_Full);
}

int Keyboard::has_output()
{
	return (status() & Status_Output_Buffer_Full);
}

int Keyboard::selftest_success()
{
	return (status() & Status_Selftest_Success);
}

int Keyboard::is_locked()
{
	return !(status() & Status_Locked);
}

void Keyboard::write_cmd(unsigned char cmd)
{
	IO::outb(0x64, cmd);
}

unsigned char Keyboard::read_data()
{
	return IO::inb(0x60);
}

void Keyboard::write_data(unsigned char data)
{
	IO::outb(0x60, data);
}


