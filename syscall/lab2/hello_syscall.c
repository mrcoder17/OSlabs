#include <unistd.h>
#include <sys/syscall.h>

ssize_t my_write(int fd, const void *buf, size_t count) {
    long ret;
    asm volatile (
        "syscall"
        : "=a" (ret) // RAX, return value
        : "0" (__NR_write), //num write, RAX
		 "D" (fd),  //RDI
		 "S" (buf), //RSI
		 "d" (count)//RDX
        : "cc", "rcx", "r11", "memory" //options for connect "syscall"
    );
    return (ssize_t) ret;
}

int main(){
    my_write(1, "Hello World\n", 12);
    return 0;
}
