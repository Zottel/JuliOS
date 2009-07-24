
struct memory_header
{
	size_t size;
	struct memory_header *prev;
	struct memory_header *next;
	
	/* Chunk states:
	 *  0: Free chunk.
	 *  1: Chunk allocated.
	 *  2: Chunk reserved.
	 */
	u8int state;
};

class Memory
{
	private:
		struct memory_header *head, *bottom;
		struct memory_header *merge_chunks(struct memory_header *element);
		
	public:
		Memory();
		void register_pool(void *base, size_t length);
		void free(void * ptr);
		void *allocate(size_t size);
		
		void *allocopy(void *ptr, size_t size);
		
#ifdef DEBUG
		void debug(Screen *screen);
#endif
};

void set_memory_handler(Memory *mem);

void *malloc(size_t);
void free(void *p);

void *memset (void *s, int c, size_t n);

void * operator new (size_t size);
void * operator new[] (size_t size);

void operator delete (void *p);
void operator delete[] (void *p);
