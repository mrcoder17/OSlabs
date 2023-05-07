#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BUFFER_SIZE 256

int global_init = 10;
int global_uninit;
const int global_const = 5;


void pairing_maps_var_addresses(const char* maps_path, void* var_address){
    FILE *maps_file = fopen(maps_path, "r");
    if (!maps_file) {
        perror("fopen err");
        exit(1);
    }

    char buf[BUFFER_SIZE];

    while (fgets(buf, BUFFER_SIZE, maps_file)) {
        uintptr_t start_addr;
        uintptr_t end_addr;
        char perm[5];
        sscanf(buf, "%lx-%lx %4s", &start_addr,&end_addr,perm);
        if (var_address >= (void*)start_addr && var_address <= (void*)end_addr) {
            printf("address:%p  in range %lx-%lx perm: %s\n", var_address, start_addr, end_addr, perm);
            break;
        }
    }
    fclose(maps_file);
}

void print_addresses() {
    const char* maps_path = "/proc/self/maps";

    int local_init_var = 20;
    int local_uninit_var;
    static int static_var = 30;
    const int const_var = 40;
    printf("var: local_init_var \t");
    pairing_maps_var_addresses(maps_path,(void*)&local_init_var);
    printf("var: local_uninit_var \t");
    pairing_maps_var_addresses(maps_path,(void*)&local_uninit_var);
    printf("var: static_var \t");
    pairing_maps_var_addresses(maps_path,(void*)&static_var);
    printf("var: const_var \t\t");
    pairing_maps_var_addresses(maps_path,(void*)&const_var);
    printf("var: global_init \t");
    pairing_maps_var_addresses(maps_path,(void*)&global_init);
    printf("var: global_uninit \t");
    pairing_maps_var_addresses(maps_path,(void*)&global_uninit);
    printf("var: global_const \t");
    pairing_maps_var_addresses(maps_path,(void*)&global_const);
}

int main() {
    print_addresses();
    printf("\npid: %d\n", getpid());
    sleep(1200);
    return 0;
}
