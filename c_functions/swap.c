#include <stdlib.h>
#include <stdio.h>

void swap(int *x, int *y);

int main(){
    int x, y;
    x = 10;
    y = 20;
    printf("Started values...\n");
    printf("(%d, %d)\n", x, y);
    printf("(%p, %p)\n", &x, &y);
    swap(&x, &y); // Pass the memory adress
    printf("(%d, %d)\n", x, y);
    return 0;
}

void swap(int *x, int *y){
    printf("Inside swap function...\n");
    printf("(%d, %d)\n", *x, *y);
    printf("(%p, %p)\n", x, y);
    // *x = 0x12345, *y = 0x98765
    int temp = *x;
    *x = *y;
    *y = temp;
}