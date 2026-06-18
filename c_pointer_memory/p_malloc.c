#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* string_function(char *astring){
    char *s;
    s = (char*)malloc(strlen(astring) + 1);
    if(s == NULL){
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    strcpy(s, astring);
    return s;
}

int main(int argc, char **argv){
    printf("%s\n", string_function("Fred"));
    printf("%s\n", string_function("Bjarne Stroustrup"));
    return 0;
}