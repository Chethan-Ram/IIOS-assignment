#include <stdio.h>
#include<stdint.h>
// Total memory size
#define MEMORY_SIZE(100 *1024)
// Minimum alocatable memory
#define MIN_ALLOC_SIZE 1
// Maximum allocatable memory
#define MAX_ALLOC_SIZE MEMORY_SIZE

static uint8_t memory[MEMORY_SIZE];

typedef struct Block{ // Structure for RAM block
	size_t size;
	int free;
	struct Block *next;
}Block;
// Define block size
#define BLOCK_SIZE sizeof(Block) 

static Block *free_list = (Block *)memory;

void init_memory(void){
	free_list->size = MEMORY_SIZE - BLOCK_SIZE;
	free_list->free =1;
	free_list->next=NULL;
}
int *allocate(int size){ // Allocate memory function
	if(size < MIN_ALLOC_SIZE || size >MAX_ALLOC_SIZE){
		return NULL;
	}
	Block *curr =free_list;
	while(curr){
		if (curr->free && curr->size >= (size_t)size){
			if(curr->size >= size +BLOCK_SIZE +MIN_ALLOC_SIZE){
				Block *new_block =(Block *)((uint8_t *)curr + Block_SIZE + size);
				new_block->size = curr->size -size -BLOCK_SIZE;
				new_block->free=1;
				new_block->next =curr->next;
				curr->size =size;
				curr->next = new_block;
			}
			curr->free=0;
			return (int *)((uint8_t *)curr + BLOCK_SIZE);
		}
		curr = curr->next;
	}
	return NULL;

}
void deallocate(int *ptr){ // Deallocate memory function
	if(!ptr)return;
	Block *block =(Block *)((uint8_t *)ptr -BLOCK_SIZE);
	block->free=1;
	Block *curr =free_list;
	while(curr && curr->next){
		if(curr->free && curr->next->free){
			curr->size +=BLOCK_SIZE + curr->next->size;
			curr->next =curr->next->next;
		}else{
			curr =curr->next;
		}
	}
}
int main(void)
{
	init_memory();

	int *mem[100];
	mem[0]=allocate(128);
	mem[1]=allocate(1024);
	mem[2]=allocate(4096);

	deallocate(mem[1]);
	mem[1] = allocate(512);

	printf("mem[0] = %p\n",(void *)mem[0]);
	printf("mem[1] = %p\n",(void *)mem[1]);
	printf("mem[2] = %p\n",(void *)mem[2]);

	return 0;
}
