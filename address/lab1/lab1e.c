#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void heap_operations() {
    char* buffer = (char*)malloc(100 * sizeof(char));
    strcpy(buffer, "hello world");
    printf("start buf:%s\n", buffer);
    free(buffer);

    printf("buf after free: %s\n", buffer);
    buffer = (char*)malloc(100 * sizeof(char));
    strcpy(buffer, "hello world");
    printf("new buf: %s\n", buffer);

    char* mid_ptr = buffer + (strlen(buffer) / 2);
    printf("ptr in mid: %s\n", mid_ptr);
    free(mid_ptr);
    printf("buf after free(mid_prt): %s\n", buffer);
    free(buffer);
}

int main() {
    heap_operations();
}