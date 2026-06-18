#include "functions.h"
#include <stdio.h>

#define PI 3.14159 // constant
#define CIRCLE_AREA(radius) (PI * (radius) * (radius)) // macro

int main(){
    printf("The radius is: %f\n", CIRCLE_AREA(PI));
    hello();
    return 0;
}