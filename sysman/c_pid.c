#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    int pid = fork();

    if(pid == 0){
        printf("I am the child process with PID: %d\n", getpid());
    }else{
        wait(NULL);
        printf("I am the parent process with PID: %d\n", getpid());
    }

    return 0;
}