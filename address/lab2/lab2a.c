#include <stdio.h>
#include <unistd.h>

int main() {
    printf("pid: %d\n", getpid());
    sleep(1);
    execv("/proc/self/exe", NULL);
    printf("Hello world\n");
    return 0;
}