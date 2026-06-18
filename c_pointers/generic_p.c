#include <stdio.h>

#define LEN 3

int main(int argc, char **argv){
    char* word[LEN];
    word[0] = "Hello ";
    word[1] = "from ";
    word[2] = "Maxine\n";

    void* pgen;

    for(int i = 0; i < LEN; i++){
        pgen = word[i];
        printf("%s", (char*)pgen);
    }

    return 0;
}