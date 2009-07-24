
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
	u8int granularity;
	u8int base_high;
}
__attribute__((packed));

class GDT
{
	private:
		u16int used_entries;
		struct gdt_entry entries[32] __attribute__ ((aligned (8)));
		void addSegment (u32int base, u32int limit, u8int present, u8int system, u8int type, u8int privLevel, u8int granularity);
		
	public:
		GDT();
		void debug(Screen *Screen);
		void addDataSegment (u32int base, u32int limit, u8int readWrite, u8int expandDown, u8int privLevel, u8int byteGranularity);
		void addCodeSegment (u32int base, u32int limit, u8int readExecute, u8int privLevel, u8int byteGranularity);

		void install();
};
