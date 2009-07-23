class IO
{
	public:
		static void wait(void);
		static unsigned char inb(unsigned short port);
		static void outb(unsigned short port, unsigned char val);
};
