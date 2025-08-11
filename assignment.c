#include <stdio.h>
// Total memory size
#define MEMORY_SIZE   (100*1024)
#define MAX_POINTERS  100

unsigned char memory[MEMORY_SIZE];
void* mem[MAX_POINTERS];
int size_arr[MAX_POINTERS];
int used = 0;

// Function to show remaining free memory
void print_remaining() {
    printf("Available memory: %d bytes\n", MEMORY_SIZE - used);
}

void memset(void *ptr, int value, int count) {
    unsigned char *p = (unsigned char*)ptr;
    for (int i = 0; i < count; i++) {
        p[i] = (unsigned char)value;
    }
}

void memmove(void *dest, const void *src, int count) {
    unsigned char *d = (unsigned char*)dest;
    const unsigned char *s = (const unsigned char*)src;

    if (d < s) {
        for (int i = 0; i < count; i++) {
            d[i] = s[i];
        }
    } else {
        for (int i = count - 1; i >= 0; i--) {
            d[i] = s[i];
        }
    }
}

void* allocate(int n, int size) {       // Allocate memory function
    if (n < 0 || n >= MAX_POINTERS) {
        printf("Index out of range. Please use 0 to 99.\n");
        print_remaining();
        return NULL;
    }
    if (size <= 0 || size > MEMORY_SIZE - used) {
        printf("Not enough memory available.\n");
        print_remaining();
        return NULL;
    }
    if (mem[n] != NULL) {
        printf("memory[%d] is already used! Please free it before reuse.\n", n);
        print_remaining();
        return NULL;
    }

    void* ptr = (void*)(memory + used);
    mem[n] = ptr;
    size_arr[n] = size;
    used += size;

    printf("Reserved %d bytes at mem[%d]\n", size, n);
    print_remaining();
    return ptr;
}

void deallocate(int n) {            // De allocate memory function
    if (n < 0 || n >= MAX_POINTERS || mem[n] == NULL) {
        printf("Invalid or unallocated index: %d\n", n);
        print_remaining();
        return;
    }

    int sz = size_arr[n];
    unsigned char *block_start = (unsigned char*)mem[n];
    int bytes_after = used - ((block_start + sz) - memory);

    if (bytes_after > 0) {
        memmove(block_start, block_start + sz, bytes_after);
    }

    for (int i = n + 1; i < MAX_POINTERS; ++i) {
        if (mem[i]) {
            mem[i] = (unsigned char*)mem[i] - sz;
        }
    }

    mem[n] = NULL;
    size_arr[n] = 0;
    used -= sz;
    printf("Freed mem[%d] of %d bytes. Memory compacted.\n", n, sz);
    print_remaining();
}

void print_state() {            // Debug: print memory map
    printf("Memory in use: %d bytes\n", used);
    print_remaining();
    for (int i = 0; i < MAX_POINTERS; ++i) {
        if (mem[i]) {
            printf(" mem[%d]: %p, size %d\n", i, mem[i], size_arr[i]);
        }
    }
    printf("\n");
}

int main() {
    // initialize everything
    for (int i = 0; i < MAX_POINTERS; i++) {
        mem[i] = NULL;
        size_arr[i] = 0;
    }
    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = 0;
    }

    for (;;) {
        int choice;
        printf("\n1. Allocate\n2. Deallocate\n3. Print the state\n4. Exit\nEnter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exit\n");
            break;
        }
        if (choice == 4) {
            break;
        }
        if (choice == 1) {
            int n, size;
            printf("Enter index (mem[n],(<100): ");
            scanf("%d", &n);
            printf("Enter byte count to allocate: ");
            scanf("%d", &size);
            allocate(n, size);
        } else if (choice == 2) {
            int n;
            printf("Enter index (mem[n]) to free: ");
            scanf("%d", &n);
            deallocate(n);
        } else if (choice == 3) {
            print_state();
        } else {
            printf("Unknown choice.\n");
        }
    }

    return 0;
}
