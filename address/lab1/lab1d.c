#include <stdio.h>

int* address_local() {
    int localVar = 333;
    return &localVar;
}

int main() {
    int* ptr = address_local();
    printf("Value at the returned address: %d\n", *ptr);
    return 0;
}