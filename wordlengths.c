#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define MAX_wordsArray 1000
#define maxWordLength 100

void histogram(int *x, double *y, int n, int width);
int *histogram_lengths(char **strings, int n);

int main(int argc, char* argv[]) { 
// reads file and creates array of all the words
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    char **wordsArray = NULL;
    char buffer[maxWordLength]; // buffer for each line in file
    int wordCount = 0; // for array manipulation
    while (fscanf(file, "%99s", buffer) == 1) { // while there are still things to scan
        char **temp = realloc(wordsArray, (wordCount + 1) * sizeof(char *)); // dynamically allocates memory so wordsarray can grow no matter how many words in the txt file
        if (temp == NULL) {
            printf("Memory allocation failed\n");
            // Free previously allocated memory before exiting
            for (int i = 0; i < wordCount; i++) {
                free(wordsArray[i]);
            }
            free(wordsArray);
            fclose(file);
            return 1;
        }
        wordsArray = temp;
        wordsArray[wordCount] = malloc(strlen(buffer) + 1); // allocates memory for each individual word
        if (wordsArray[wordCount] == NULL) {
            printf("Memory allocation failed\n");
            // Free previously allocated memory before exiting
            for (int i = 0; i < wordCount; i++) {
                free(wordsArray[i]);
            }
            free(wordsArray);
            fclose(file);
            return 1;
        }
        strcpy(wordsArray[wordCount], buffer); // stores word in wordsArray
        wordCount++;
    }
    fclose(file); // close file
    
    //finds longest word in the array
    int max_length = 0;
    for (int i = 0; i < wordCount; i++) {
        int length = strlen(wordsArray[i]);
        if (length > max_length) {
            max_length = length;
        }
    }

    // make an array of word lengths (ints 0 to max_length)
    int *lengths = malloc((max_length + 1) * sizeof(int));
    for (int i = 0; i <= max_length; i++) {
        lengths[i] = i;
    }

    //create and convert array of frequencies
    int *frequencyOfLength = histogram_lengths(wordsArray, wordCount);
    double *freqDoubles = malloc((max_length + 1) * sizeof(double));
    for (int i = 0; i <= max_length; i++) {
        freqDoubles[i] = (double)frequencyOfLength[i];
    }

    //print out the histogram
    histogram(lengths, freqDoubles, max_length + 1, 100);

    // Free the allocated memory
    for (int i = 0; i < wordCount; i++) {
        free(wordsArray[i]);
    }
    free(wordsArray);
    free(frequencyOfLength);

    return 0;
}