//Написать программу hello.c, которая выводит фразу “Hello world”:
//a. получить исполняемый файл;
//b. посмотреть unresolved symbols (puts, printf) с помощью nm;
//c. посмотреть зависимости (ldd);
//d. запустить.

#include <stdio.h>

int main () {
   printf("Hello world\n");
   return 0;
}

