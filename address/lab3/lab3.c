#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>

#define REGION_SIZE 4096

typedef struct Block {
    size_t size;
    struct Block* next;
    struct Block* prev;
    int is_free;
} Block;

Block* free_list = NULL;
void* initial_region = NULL;

void* my_malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    Block* current = free_list;
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            if (current->size > size + sizeof(Block)) {
                Block* new_block = (Block*)((char*)current + sizeof(Block) + size);
                new_block->size = current->size - size - sizeof(Block);
                new_block->is_free = 1;
                new_block->next = current->next;
                new_block->prev = current;
                if (current->next != NULL) {
                    current->next->prev = new_block;
                }
                current->next = new_block;
                current->size = size;
            }
            current->is_free = 0;
            return (void*)((char*)current + sizeof(Block));
        }
        current = current->next;
    }

    return NULL;
}

void my_free(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    Block* block = (Block*)((char*)ptr - sizeof(Block));
    block->is_free = 1;

    if (block->prev != NULL && block->prev->is_free) {
        block->prev->size += block->size + sizeof(Block);
        block->prev->next = block->next;
        if (block->next != NULL) {
            block->next->prev = block->prev;
        }
        block = block->prev;
    }

    if (block->next != NULL && block->next->is_free) {
        block->size += block->next->size + sizeof(Block);
        block->next = block->next->next;
        if (block->next != NULL) {
            block->next->prev = block;
        }
    }
}

void initialize_region() {
    initial_region = mmap(NULL, REGION_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (initial_region == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    free_list = (Block*)initial_region;
    free_list->size = REGION_SIZE - sizeof(Block);
    free_list->is_free = 1;
    free_list->next = NULL;
    free_list->prev = NULL;
}

void cleanup_region() {
    if (initial_region != NULL) {
        munmap(initial_region, REGION_SIZE);
        initial_region = NULL;
        free_list = NULL;
    }
}

void test1(){
    void* buffer1 = my_malloc(100);
    void* buffer2 = my_malloc(100);
    void* buffer3 = my_malloc(100);
    void* buffer4 = my_malloc(100);

    printf("test 1 buffer1: %p\n", buffer1);
    printf("test 1 buffer2: %p\n", buffer2);
    printf("test 1 buffer3: %p\n", buffer3);
    printf("test 1 buffer4: %p\n", buffer4);

    my_free(buffer2);
    my_free(buffer3);

    void* buffer5 = my_malloc(150);
    printf("test 1 buffer5: %p\n", buffer5);

    my_free(buffer1);
    my_free(buffer4);
    my_free(buffer5);
}

void test2(){
    void* buffer1 = my_malloc(100);
    void* buffer2 = my_malloc(100);
    void* buffer3 = my_malloc(100);
    void* buffer4 = my_malloc(100);

    printf("test 2 buffer1: %p\n", buffer1);
    printf("test 2 buffer2: %p\n", buffer2);
    printf("test 2 buffer3: %p\n", buffer3);
    printf("test 2 buffer4: %p\n", buffer4);

    my_free(buffer2);
    my_free(buffer3);

    void* buffer5 = my_malloc(250);
    
    printf("test 2 buffer5: %p\n", buffer5);

    my_free(buffer1);
    my_free(buffer4);
    my_free(buffer5);
}

int main() {
    initialize_region();

    printf("test1. 150 by my_malloc\n");
    test1();
    printf("test2. 250 by my_malloc\n");
    test2();

    cleanup_region();
    return 0;
}
