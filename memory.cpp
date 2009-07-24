#define DEBUG 1

#include <types.h>
#ifdef DEBUG
	#include <screen.h>
#endif

#include "memory.h"

Memory *default_mem;

Memory::Memory()
{
	head = NULL;
	bottom = NULL;
}

struct memory_header *Memory::merge_chunks(struct memory_header *element)
{
	while(element->prev && element->prev->state == 0 && (((u32int) (element->prev + 1)) + element->prev->size) == ((u32int) element))
	{
		element->prev->next = element->next;
		
		if(element->next)
		{
			element->next->prev = element->prev;
		}
		else
		{
			bottom = element->prev;
		}
		
		element->prev->size += sizeof(struct memory_header) + element->size;
		element = element->prev;
	}
	
	while(element->next && element->next->state == 0 && (((u32int) (element + 1)) + element->size) == ((u32int) element->next))
	{
		element->size += sizeof(struct memory_header) + element->next->size;
		
		if(element->next->next)
		{
			element->next->next->prev = element;
		}
		
		element->next = element->next->next;
	}
	
	return element;
}

void Memory::register_pool(void *base, size_t length)
{
	struct memory_header *new_header, *runner;
	
	if(length > sizeof(memory_header))
	{
		new_header = (memory_header *) base;
		new_header->state = 0;
		new_header->size = length - sizeof(struct memory_header);
		
		new_header->prev = NULL;
		new_header->next = NULL;
		
		if(head == NULL)
		{
			head = new_header;
			bottom = new_header;
		}
		else if(new_header > bottom)
		{
			bottom->next = new_header;
			new_header->prev = bottom;
			bottom = new_header;
		}
		else if(new_header < head)
		{
			head->prev = new_header;
			new_header->next = head;
			head = new_header;
		}
		else
		{
			runner = head;
			while(runner->next && runner < new_header)
			{
				if(new_header > runner && new_header < runner->next)
				{
					runner->next->prev = new_header;
					new_header->next = runner->next;
					runner->next = new_header;
					new_header->prev = runner;
				}
				runner = runner->next;
			}
		}
		
		merge_chunks(new_header);
	}
	else
	{
		//TODO: check for chunks before it and append to them.
		return;
	}
}

void Memory::free(void * ptr)
{
	struct memory_header *current = ((struct memory_header *) ptr) - 1;
	
	// Mark the chunk as not allocated.
	current->state = 0;
	
	// Merge with neighbouring chunks.
	merge_chunks(current);
}

void *Memory::allocate(size_t size)
{
	struct memory_header *runner = head, *new_header;
	
	while(runner)
	{
		
		if(runner->state == 0 && runner->size >= size)
		{
			if(runner->size - (size + sizeof(struct memory_header)) > 0)
			{
				
				new_header = (struct memory_header *)(((unsigned long int)(runner + 1)) + size);
				
				new_header->next = runner->next;
				runner->next = new_header;
				new_header->prev = runner;
				if(new_header->next)
				{
					new_header->next->prev = new_header;
				}
				
				new_header->size = runner->size - (size + sizeof(struct memory_header));
				runner->size = size;
				
				new_header->state = 0;
			}
			
			runner->state = 1;
			u32int i; unsigned char * nuller = (unsigned char *)(runner + 1);
			for(i = 0; i < runner->size; i++)
			{
				*nuller++ = '\0';
			}
			return (void *)(runner + 1);
		}
		runner = runner->next;
	}
	return NULL;
}

void *Memory::allocopy(void *ptr, size_t size)
{
	unsigned char *old_ptr = (unsigned char *)ptr;
	void *new_ptr = allocate(size);
	unsigned char *runner = (unsigned char *)new_ptr;
	
	if(new_ptr)
	{
		size_t i;
		for(i = 0; i < size; i++)
		{
			*runner++ = *old_ptr++;
		}
	}
	return new_ptr;
}

#ifdef DEBUG
void Memory::debug(Screen *screen)
{
	struct memory_header *runner = head;
	screen->printf("\nMemory debugger\n");
	while(runner)
	{
		screen->printf("  chunk at 0x%x has size 0x%x and is %s.\n", runner, runner->size, runner->state ? "allocated" : "free");
		runner = runner->next;
	}
}
#endif

void set_memory_handler(Memory *mem)
{
	default_mem = mem;
}

void *malloc(size_t size)
{
	return default_mem->allocate(size);
}

void free(void *p)
{
	default_mem->free(p);
}

void * operator new (size_t size)
{
	return default_mem->allocate(size);
}

void * operator new[] (size_t size)
{
	return default_mem->allocate(size);
}

void operator delete (void *p)
{
	default_mem->free(p);
}

void operator delete[] (void *p)
{
	default_mem->free(p);
}

void *memset (void *p, int c, size_t n)
{
	u8int *cur = (u8int *) p;
	while (n > 0) {
		*cur = c;
		cur++;
		n--;
	}
	return p;
}
