#include <stdio.h>
#include <stdlib.h>

int main(void){
    char arr[] = { 'a', 'p', 'p', 'l', 'e', '\n', '\0'};
    int dim = sizeof(arr);
    printf("The array size is: %d\n", dim);

    for(int i = 0; i < dim; i++){
        printf("%c", arr[i]);
    }

    for(int i = 0; i < dim; i++){
        printf("%c", i[arr]);
    }

    for(int i = 0; i < dim; i++){
        printf("%c", *(arr + i));
    }

    return 0;
}