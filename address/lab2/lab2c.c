#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>

void allocate_on_stack(){
    char buffer[4096];
    allocate_on_stack();
}

void allocate_on_heap() {
    while (1) {
        char* buffer = malloc(4096);
        sleep(1);
        free(buffer);
    }
}

void sigsegv_handler(int signum) {
    printf("Received SIGSEGV signal.\n");
    exit(1);  // Выход из программы после обработки сигнала
}

int main() {
    printf("PID: %d\n", getpid());
    sleep(15);

//    allocate_on_stack();
//    allocate_on_heap();

    void* mmap_ptr = mmap(NULL, 10 * getpagesize(), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mmap_ptr == MAP_FAILED) {
        perror("mmap error");
        exit(1);
    }

    printf("mmap ptr %p\n", mmap_ptr);
    signal(SIGSEGV, sigsegv_handler);

    if (mprotect(mmap_ptr, 10 * 4096, PROT_NONE) == -1) {
        perror("mprotect err");
        exit(1);
    }
//    printf("trying to read from protected region: %c\n", *((char*)mmap_ptr));

    if (mprotect(mmap_ptr, 10 * 4096, PROT_READ) == -1) {
        perror("mprotect err");
        exit(1);
    }
//    printf("trying to write to protected region: ");
//    *((char*)mmap_ptr) = 'A';

    if (munmap(mmap_ptr + 3 * getpagesize(), 3 * getpagesize()) == -1) {
        perror("munmap error");
        exit(1);
    }

    printf("pages detached\n");
    sleep(300);
    return 0;
}
