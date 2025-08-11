#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

// Total memory size
#define MEM_SIZE   (100 * 1024) 
// Define num block  
#define NUM_BLOCKS 128
// Define block size
#define BLOCK_SIZE (MEM_SIZE / NUM_BLOCKS)

static unsigned char ram[MEM_SIZE];        
static bool used[NUM_BLOCKS] = {false};     
static void *blkAddr[NUM_BLOCKS] = {NULL}; 
static int blkCount[NUM_BLOCKS] = {0};      


void* allocate(size_t bytes) {		// Allocate memory function
    if (bytes == 0 || bytes > MEM_SIZE) return NULL;

    int blocks_needed = (bytes + BLOCK_SIZE - 1) / BLOCK_SIZE;
    int free_count = 0, start_idx = -1;

    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (!used[i]) {
            if (start_idx == -1) start_idx = i;
            if (++free_count == blocks_needed) {
                for (int j = start_idx; j < start_idx + blocks_needed; j++) {
                    used[j] = true;
                    blkAddr[j] = ram + j * BLOCK_SIZE;
                }
                blkCount[start_idx] = blocks_needed;
                return blkAddr[start_idx];
            }
        } else {
            start_idx = -1;
            free_count = 0;
        }
    }
    return NULL; // No space
}


void deallocate(void *ptr) {				// Deallocate memory function
    if (!ptr) return;
    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (blkAddr[i] == ptr) {
            for (int j = i; j < i + blkCount[i]; j++) {
                used[j] = false;
                blkAddr[j] = NULL;
            }
            blkCount[i] = 0;
            return;
        }
    }
}

// Debug: print memory map
void print_mem_map(void) {
    printf("Memory Map: ");
    for (int i = 0; i < NUM_BLOCKS; i++)
        printf("%c", used[i] ? '#' : '.');
    printf("\n");
}

int main(void) {
    void *mem[100];

    
    printf("Test case 1:\n");
    mem[0] = allocate(128);
    mem[1] = allocate(1024);
    mem[2] = allocate(4096);
    printf("Allocated:\n%p\n%p\n%p\n", mem[0], mem[1], mem[2]);
    print_mem_map();

    deallocate(mem[1]);
    mem[1] = allocate(512);
    printf("Re-allocated:\n%p\n", mem[1]);
    print_mem_map();

    deallocate(mem[0]);
    deallocate(mem[1]);
    deallocate(mem[2]);
    print_mem_map();


    printf("\nTest case 2:\n");
    mem[0] = allocate(1);
    if (mem[0]) printf("Allocated 1B: %p\n", mem[0]);
    else printf("1B allocation failed\n");
    deallocate(mem[0]);

    mem[0] = allocate(MEM_SIZE);
    if (mem[0]) printf("Allocated 100KB: %p\n", mem[0]);
    else printf("100KB allocation failed\n");
    deallocate(mem[0]);

    print_mem_map();
    return 0;
}
