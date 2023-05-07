#include <stdio.h>
#include <stdlib.h>
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

Block* get_last_region() {
    Block* current = free_list;
    while (current->next != NULL) {
        current = current->next;
    }
    return current;
}

Block* find_free_block(size_t size) {
    Block* current = free_list;
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void* my_malloc(size_t size) {
    size_t block_size = size + sizeof(Block);
    Block* block = find_free_block(block_size);

    if (block) {
        block->is_free = 0;
        return (void*)((char*)block + sizeof(Block));
    }

    size_t region_size = block_size > REGION_SIZE ? block_size : REGION_SIZE;
    Block* new_region = mmap(NULL, region_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (new_region == MAP_FAILED) {
        return NULL;
    }

    new_region->size = region_size - sizeof(Block);
    new_region->is_free = 0;
    new_region->next = NULL;
    new_region->prev = NULL;

    if (!free_list) {
        free_list = new_region;
    } else {
        Block* last_region = get_last_region();
        last_region->next = new_region;
        new_region->prev = last_region;
    }

    return (void*)((char*)new_region + sizeof(Block));
}

void my_free(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    Block* block = (Block*)ptr - 1;
    block->is_free = 1;

    if (block->prev && block->prev->is_free) {
        block->prev->size += block->size + sizeof(Block);
        block->prev->next = block->next;
        if (block->next) {
            block->next->prev = block->prev;
        }
        block = block->prev;
    }

    if (block->next && block->next->is_free) {
        block->size += block->next->size + sizeof(Block);
        block->next = block->next->next;
        if (block->next) {
            block->next->prev = block;
        }
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

void test3(){
    void* buffer1 = malloc(100);
    void* buffer2 = malloc(100);
    void* buffer3 = malloc(100);
    void* buffer4 = malloc(100);

    printf("test 1 buffer1: %p\n", buffer1);
    printf("test 1 buffer2: %p\n", buffer2);
    printf("test 1 buffer3: %p\n", buffer3);
    printf("test 1 buffer4: %p\n", buffer4);

    free(buffer2);
    free(buffer3);

    void* buffer5 = malloc(150);
    printf("test 1 buffer5: %p\n", buffer5);
    free(buffer1);
    free(buffer4);
    free(buffer5);
}

void test4(){
    void* buffer1 = malloc(100);
    void* buffer2 = malloc(100);
    void* buffer3 = malloc(100);
    void* buffer4 = malloc(100);

    printf("test 2 buffer1: %p\n", buffer1);
    printf("test 2 buffer2: %p\n", buffer2);
    printf("test 2 buffer3: %p\n", buffer3);
    printf("test 2 buffer4: %p\n", buffer4);

    free(buffer2);
    free(buffer3);

    void* buffer5 = malloc(250);
    printf("test 2 buffer5: %p\n", buffer5);
    free(buffer1);
    free(buffer4);
    free(buffer5);
}

int main() {
    printf("test 1. 150 by my_malloc\n");
    test1();

    printf("test 2. 250 by my_malloc\n");
    test2();

    printf("test 3. 150 by malloc\n");
    test3();

    printf("test 4. 250 by malloc\n");
    test4();

    printf("\n%d\n", getpid());
    sleep(1000);
    return 0;
}