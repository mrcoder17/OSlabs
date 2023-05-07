#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>

int main (int argc, char** argv){
    if(argc == 1){
        perror ("nothing to cat");
        exit(1);
    }
    int status = 0;
    pid_t my_id = fork();
    struct user_regs_struct regs;

    switch(my_id){
        case -1:
            perror("fork err");
            exit(1);

        case 0:
            ptrace(PTRACE_TRACEME, 0, NULL, NULL);
            execl("/bin/cat", "cat", argv[1], (char*) 0);
            perror("execl err");
            exit(1);

        default:
            if (wait(&status) == -1){
	            perror("wait error");
                exit(1);
            }

            while (!WIFEXITED(status)) {
                if (ptrace(PTRACE_SYSCALL, my_id, NULL, NULL) == -1) {
                    perror("ptrace syscall err");
                    exit(1);
                }

		        if (wait(&status) == -1){
                    perror("wait error");
                    exit(1);
                }

       	        if (WIFEXITED(status) || WIFSIGNALED(status)) break;

                if (ptrace(PTRACE_GETREGS, my_id, NULL, &regs) == -1) {
                    perror("ptrace getregs err");
                    exit(1);
                }
                printf("syscall %llu\n", regs.orig_rax);
            }
    }
    return 0;
}
