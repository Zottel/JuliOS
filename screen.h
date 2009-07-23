class Screen
{
	private:
		char *video;
		int x, y;
		int size_x, size_y;
		unsigned char color;
		//const int x_size = 80;
		//const int y_size = 25;
	public:
		Screen(char * position, int x_size, int y_size);
		void setcolor(unsigned char color);
		void clear();
		void putcharxy(char char_to_put, int x, int y);
		void newline();
		
		void print_char(char char_to_put);
		void print_string(const char *string);
		void print_string(char *string);
		void print_number(unsigned long int number, unsigned int base);
		void print_number(signed long int number, unsigned int base);
		
		void printf(const char *format, ...);
};
