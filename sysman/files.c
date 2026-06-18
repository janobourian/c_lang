#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(){
    char message[] = "Hello world\n";
    write(STDOUT_FILENO, message, sizeof(message) - 1);
    int fd = open("information.txt", O_RDWR | O_CREAT, 0755);

    if(fd == -1){
        perror("Error opening file");
        exit(1);
    }

    int buffer_size = 1;
    char buffer[buffer_size];
    int content = read(fd, buffer, buffer_size);

    while(content > 0){
        write(STDOUT_FILENO, buffer, content);
        content = read(fd, buffer, buffer_size);
    }

    close(fd);

    char text[] = "Hello from my program\n";

    fd = open("me.txt", O_WRONLY | O_CREAT, 0755);
    lseek(fd, 0, SEEK_END);
    write(fd, text , sizeof(text) - 1);
    close(fd);

    return 0;
}
