#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float get_value(float num)
{
    return pow(num, 3.0f) - 3;
}

float get_middle_point(float a, float b)
{
    return (a + b) / 2.0f;
}

int main(void)
{
    int a, b;
    float tol, middle_point, fa, fb, fc;

    printf("Insert inf lim: ");
    scanf("%d", &a);

    printf("Insert sup lim: ");
    scanf("%d", &b);

    printf("Insert tolerance: ");
    scanf("%f", &tol);

    while ((b - a) * (b - a) > tol) {
        middle_point = get_middle_point((float)a, (float)b);

        fa = get_value((float)a);
        fb = get_value((float)b);
        fc = get_value(middle_point);

        if (fa * fc < 0.0f) {
            b = (int)middle_point;
        } else if (fb * fc < 0.0f) {
            a = (int)middle_point;
        } else {
            break;
        }
    }

    printf("The middle point between %d and %d is %f\n", a, b, middle_point);
    return 0;
}