#include <stdio.h>
#include <string.h>

int main(){
    char name[] = "John Doe";
    /* You can write
    char name[11] = "John Doe";
    but do not forget the extra space for ending character
    */
    printf("His name is %s with length %lu\n", name, strlen(name));
    return 0;
}