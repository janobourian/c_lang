#include <stdio.h>

int main(){
    int num, value;
    int *numptr;

    num = 100;            // asign a value to the var
    numptr = &num;        // asign the memory address from num in numptr
    value = *numptr;      // asign the value from num in value

    printf("num: %d, memory address: %p\n", num, (void *)numptr);
    return 0;
}