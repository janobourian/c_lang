#include <stdio.h>
#define ARRAY_LEN 10

int main(void){
    int arr[ARRAY_LEN] = {-3, 4, 5, 9, 0, -1, 1, -3, 7, 1};
    int temp; 

    for(int i = 0; i < ARRAY_LEN; i++){
        for(int j = 0; j < ARRAY_LEN - 1; j++){
            if(arr[j] > arr[j+1]){
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }

    for(int i = 0; i < ARRAY_LEN; i++){
        printf("%d ", arr[i]);
    }

    printf("\n");
    return 0; 
}