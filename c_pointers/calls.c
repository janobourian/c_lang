#include <stdio.h>

void operate(int *pnum){
    printf("x memory address %p\n", pnum);
    *pnum -= 100;
}

int main(int argc, char **argv){
    int x = 120;
    printf("initial value %d\n", x);
    operate(&x);
    printf("final value %d\n", x);
    return 0;
}