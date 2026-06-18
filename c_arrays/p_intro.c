#include <stdio.h>

int main(void){
    int x, y;
    int *xptr, *yptr;

    x = 10;
    y = 20;

    printf("x value: %d\n", *xptr);
    printf("y value: %d\n", *yptr);

    xptr = &x;
    yptr = &y;

    printf("x memory direction: %p\n", (void *)xptr);
    printf("y memory direction: %p\n", (void *)yptr);

    *xptr = 201;
    *yptr = 202;

    printf("x new value: %d\n", *xptr);
    printf("y new value: %d\n", *yptr);

    return 0;
}