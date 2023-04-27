#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <gnu/lib-names.h>

#include "dynamiclib.h"

int main() {
    void* h = dlopen("./libdynamic.so", RTLD_LAZY);
    if (!h) {
        printf("dlopen error %s\n", dlerror());
        return 1;
    }

    dlerror();
    void (*hello_from_dynamic_lib)();
    hello_from_dynamic_lib = dlsym(h, "hello_from_dynamic_lib");

    if (!hello_from_dynamic_lib) {
        printf("dlsyn error: %s\n", dlerror());
        dlclose(h);
        return 1;
    }

    hello_from_dynamic_lib();
    dlclose(h);
    return 0;
}
