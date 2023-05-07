#include <stdio.h>
#include <stdlib.h>

int main() {
    char* env_value = getenv("MY_ENV_VAR");
    printf("old value of MY_ENV_VAR: %s\n", env_value);
    setenv("MY_ENV_VAR", "hello world", 1);
    env_value = getenv("MY_ENV_VAR");
    printf("new value of MY_ENV_VAR: %s\n", env_value);
    return 0;
}
