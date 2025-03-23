#include <stdio.h>
#include <stdlib.h>

int *many_plays(int N);
void histogram(int *x, double *y, int n, int width);
int main() {
    int *remaining = many_plays(50000); // running 50_000 times for more accuracy
    double *remainingDouble = malloc(54 * sizeof(double));
    for (int i = 0; i < 53; i++) {
        remainingDouble[i] = (double)remaining[i];
    }
    int numbers[53];
    for (int i = 0; i < 53; i++) {
        numbers[i] = i;
    }
    histogram(numbers, remainingDouble, 53, 100);
    free(remaining);
    free(remainingDouble);
    
    return 0;
}