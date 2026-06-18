#include <stdio.h>

#define LEN 5

int main(){
    char text[] = "Hello world";
    char *context = "Lorem Ipsum";
    printf("The string is: %s, %c, %p, %p\n", text, text[0], &text, &text[0]);
    // &context is the address of the pointer variable itself, &context[0] is the address the pointer points to
    printf("The stirng is: %s, %c, %p, %p\n", context, context[0], &context, &context[0]);

    int data[LEN];
    int *pdata;
    int **ppdata;

    // Fill the array

    for(int i = 0; i < LEN; i++){
        data[i] = 100 - i*10;
    }

    pdata = data;
    ppdata = &pdata;

    for(int i = 0; i < LEN; i++){
        printf("Array value %d in position %d\n", data[i], i);
        printf("pdata value %d and pdata content %p\n", *pdata, pdata);
        printf("ppdata value %p and ppdata content %p\n", *ppdata, ppdata);
        pdata++;
    }

    int x = 8;

    printf("x value %d and x memory address %p\n", x, &x);

    int* px = &x;

    printf("p value %p and p memory address value %d\n", px, *px);

    int** ppx = &px;

    printf("pp value %p and pp memory address value %p\n", ppx, *ppx);

    return 0;
}