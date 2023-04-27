#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

void new_write(int fd, const char *buf, size_t count) {
    syscall(SYS_write, fd, buf, count);
}

int main(){
    new_write(1, "Hello World\n", 12);
    return 0;
}
