class Interrupts
{
	public:
		static void setup();
		static void enable();
		static void disable();

	private:
		static void remap();
	
};
