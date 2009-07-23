enum Keytype
{
	KEY_CHAR,
	KEY_FUNC
};

struct Key
{
	Keytype Type;
	char Char;
};


class Keyboard
{
	public:

		//Keyboard status masks:
		static const unsigned char Status_Output_Buffer_Full = 1;
		static const unsigned char Status_Input_Buffer_Full  = 2;
		static const unsigned char Status_Selftest_Success   = 4;
		static const unsigned char Status_Last_Port_Used     = 8;   //0=0x60;  1=0x61 or 0x64
		static const unsigned char Status_Locked             = 16;
		static const unsigned char Status_PSAUX              = 32;
		static const unsigned char Status_Timeout            = 64;
		static const unsigned char Status_Parity_Error       = 128;
		
		//Status retrieval
		static unsigned char status();
		static int has_input();
		static int has_output();
		static int selftest_success();
		static int is_locked();

		//Input/Output
		static void write_cmd(unsigned char cmd);
		static unsigned char read_data();
		static void write_data(unsigned char data);
};
