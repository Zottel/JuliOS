
struct gdt_header
{
	u16int limit;
	u32int address;
} __attribute__((packed));

struct gdt_entry
{
	// First Doubleword
	u16int limit_low;
	u16int base_low;

	// Second Doubleword
	u8int base_middle;
	u8int access;
	u8int flags;
	u8int base_high;
} __attribute__((packed));

class GDT
{
	private:
		struct gdt_header *header;
		u16int gdt_size;
		struct gdt_entry *entry;
		
	public:
		GDT(u16int size);
		
		void load(u16int code_descriptor);
		
		u16int create_descriptor();
		
		void descriptor_setbase(u16int descriptor, u32int base);
		void descriptor_setlimit(u16int descriptor, u32int base);
		void descriptor_settype(u16int descriptor, u8int code);
		
		void debug(Screen *Screen);
};
