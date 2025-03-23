#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

// prints a histogram given 
void histogram(int *x, double *y, int n, int width) { 
    double maxValue = y[0];
    for (int i = 0; i < n; i++) {
        if (maxValue < y[i]) {
            maxValue = y[i];
        }
    }

    for (int j = 0; j < n; j++) {
        int stars = round(((double)width / (double)maxValue) * y[j]); // calculates number of stars needed for each percentage
        printf(" %d ", x[j]);
        for (int k = 0; k < stars; k++) {
            printf("*");
        }
        printf(" %.1lf\n", y[j]);
    }
}

/*
Write another function int *histogram
 lengths(char **strings, int n) that, given an array
 of n strings strings, returns an array of integers, H, where H[i] is the number of times that strings
 of length i occurred in strings.
*/


/* 
PLAN: find longest string, allocate memory for 
the return array, loop from 1 to maxlength,
 finding the number of occurences of length i
*/
int *histogram_lengths(char **strings, int n) {
    // WORKING: ------find the length of the longest string in strings
    int max_length = 0;
    for (int i = 0; i < n; i++) {
        int lengthOfString = strlen( *(strings + i));
        if (max_length < lengthOfString) {
            max_length = lengthOfString;
        }
    }
   //allocate memory for the returned array of ints
    int *H = (int *)calloc(max_length + 1, sizeof(int)); // use calloc so all values initialise to 0

    // fill the array 
    for (int i = 0; i < n; i++) {
        int lengthOfString = strlen( *(strings + i));
        H[lengthOfString]++;
    }
    return H;
}