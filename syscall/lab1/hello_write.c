#include <unistd.h>

int main(){
    write (0, "Hello World\n", 12);
    return 0;
}
