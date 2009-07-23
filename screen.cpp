#include <types.h>
#include <screen.h>

Screen::Screen(char * position, int x_size, int y_size)
{
	video = position;
	size_x = x_size;
	size_y = y_size;
}
		
void Screen::clear()
{
	int i;
	y = (size_y - 1);
	for(i = 0; i < size_y; i++)
	{
		newline();
	}
	y = 0;
	x = 0;
}

void Screen::setcolor(unsigned char color_to_set)
{
	color = color_to_set;
}
		
void Screen::newline()
{
	//Cursor an den Anfang der nächsten Zeile setzen
	x = 0;
	y++;
	
	//Wir sind am Ende des Bildschirms angekommen, Inhalt eine Zeile nach oben schieben.
	if(y == size_y)
	{
		char * source = video + (2 * (size_x));
		char * target = video;
		int i;
		for(i = 0; i < (2 * ((size_x * 24))); i++)
		{
			*target++ = *source++;
		}
		y--;
		
		//Die letzte Zeile wird geleert.
		target = video + (2 * size_x * (size_y - 1));
		for(i = 0; i < size_x; i++)
		{
			*target++ = ' ';
			*target++ = color;
		}
	}
}

void Screen::putcharxy(char char_to_put, int x, int y)
{
	char * tmpvideo = video + (2 * ((y * size_x) + x));
	*tmpvideo = char_to_put;
}

void Screen::print_char(char char_to_put)
{
	char * tmpvideo = video + (2 * ((y * size_x) + x));
	switch(char_to_put)
	{
		case 0x0a:
			newline();
			break;
			
		default:
			*tmpvideo++ = char_to_put;
			*tmpvideo = color;
			x++;
			if(x == size_x)
			{
				newline();
			}
			break;
	}
}

void Screen::print_string(char *string)
{
	//Putchar until the null-byte is reached.
	while(*string)
	{
		print_char(*string++);
	}
}

void Screen::print_string(const char *string)
{
	Screen::print_string((char *)string);
}

void Screen::print_number(unsigned long int number, unsigned int base)
{
	const char *digits = "0123456789ABCDEFGHIJKLMNOP";
	
	char buffer[33];
	
	char *pos = ((char *) &buffer) + 32;
	*pos-- = '\0';

	//while(number || pos == ((char *) &buffer) + 31)
	while(number)
	{
		*pos-- = *(digits + (number % base));
		number = number / base;
	}
	
	print_string(++pos);
}

void Screen::print_number(signed long int number, unsigned int base)
{
	print_char(number < 0 ? '-' : '+');
	if(number < 0)
	{
		print_char('-');
		print_number((unsigned long int) (0 - number), base);
	}
	else
	{
		print_number((unsigned long int) number, base);
	}
}

void Screen::printf(const char *format, ...)
{
	unsigned long int *parameter = (unsigned long int *)&format;
	
	while(*format)
	{
		switch(*format)
		{
			case '%':
				switch(*(++format))
				{
					case '\0':
						format--;
						break;
						
					case '%':
						print_char('%');
						break;
						
					case 'b':
						print_number(*(parameter = (unsigned long *)((unsigned long)parameter + sizeof(unsigned long *))), 2);
						break;
						
					case 'o':
						print_number(*(parameter = (unsigned long *)((unsigned long)parameter + sizeof(unsigned long *))), 8);
						break;
						
					case 'd':
						print_number(*(parameter = (unsigned long *)((unsigned long)parameter + sizeof(unsigned long *))), 10);
						break;
						
					case 'h':
					case 'x':
						print_number(*(parameter = (unsigned long *)((unsigned long)parameter + sizeof(unsigned long *))), 16);
						break;
						
					case 's':
						print_string(*(const char **)(parameter = (unsigned long *)((unsigned long)parameter + sizeof(unsigned long *))));
						break;
					
					default:
						print_char('%');
						print_char(*format);
				}
				break;
			
			default:
				print_char(*format);
		}
		format++;
	}
}
