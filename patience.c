#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void shuffle(int *x, int n, int seed);
void histogram(int *x, double *y, int n, int width);
int startDeck[] = {
    1, 1, 1, 1,
    2, 3, 4, 2, 
    3, 3, 3, 3,
    4, 4, 4, 4,
    5, 5, 5, 5,
    6, 6, 6, 6,
    7, 7, 7, 7,
    8, 8, 8, 8,
    9, 9, 9, 9,
    10, 10, 10, 10,
    11, 11, 11, 11,
    12, 12, 12, 12,
    13, 13, 13, 13
};

// if the first int in return data is 1, no pairs make 11 
void add_to_11(int *visible, int n, int *returnData) {
    returnData[0] = 1;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (visible[i] + visible[j] == 11) {
                returnData[0] = 0;
                returnData[1] = i;
                returnData[2] = j;
                return;
            }
        }
    }
}

void jqk(int *visible, int numFaceUp, int *returnData) {
    int places[3] = {-1, -1, -1};
    returnData[0] = 1; // 0 means it works, 1 means it didn't
    for (int i = 0; i < numFaceUp; i++) {
        if (visible[i] == 11 && places[0] == -1) {
            places[0] = i;
        } else if (visible[i] == 12 && places[1] == -1) {
            places[1] = i;
        } else if (visible[i] == 13 && places[2] == -1) {
            places[2] = i;
        }
    }
    if (places[0] != -1 && places[1] != -1 && places[2] != -1) {
        returnData[0] = 0;
        returnData[1] = places[0];
        returnData[2] = places[1];
        returnData[3] = places[2];
        return;
    }
}



int play(int *deck, int verbose) {
    shuffle(deck, 52, -1);
    int *visible = malloc(2 * sizeof(int)); // array of cards with face up
    visible[0] = deck[0]; //initialising
    visible[1] = deck[1];
    int deckIndexer = 2; // will keep track of where we are in the deck. starts at 2 because first 2 cards are in the visible deck
    int numFaceUp = 2; // keeps track of how many cards are face up
    int gameState = 0; // 0 means win, 1 means loss
    int result11[3];
    int resultjqk[4];

    FILE *file = NULL;
    if (verbose == 0) {
    file = fopen("output.txt", "a");
    }

    while (deckIndexer < 52) {
        add_to_11(visible, numFaceUp, result11);
        if (result11[0] == 0) {
            // pairs add to 11
            if (verbose == 0) {
                fprintf(file, "%d cards face up: ", numFaceUp);
                for (int i = 0; i < numFaceUp; i++) {
                    fprintf(file, "%d ", visible[i]);
                }
                fprintf(file, "  %d and %d add to 11; cover with new cards\n", visible[result11[1]], visible[result11[2]]);
            }
            visible[result11[1]] = deck[deckIndexer]; // replace visible card by next card in deck
            visible[result11[2]] = deck[deckIndexer + 1]; // replace visible card by card after that in deck
            deckIndexer += 2; 
            continue;
        }
        
        // checks jqk, replaces cards, increments deckIndexer
        jqk(visible, numFaceUp, resultjqk);
        if (resultjqk[0] == 0) {
            // passed
            if (verbose == 0) {
                fprintf(file, "%d cards face up: ", numFaceUp);
                for (int i = 0; i < numFaceUp; i++) {    
                    fprintf(file, "%d ", visible[i]);
                }
                fprintf(file, "  J, Q, K visible; cover with new cards\n");
            }
            visible[resultjqk[1]] = deck[deckIndexer];
            visible[resultjqk[2]] = deck[deckIndexer + 1];
            visible[resultjqk[3]] = deck[deckIndexer + 2];
            deckIndexer += 3;
            continue;
        }

        if (verbose == 0) {
            fprintf(file, "%d cards face up: ", numFaceUp);
            for (int i = 0; i < numFaceUp; i++) {
                fprintf(file, "%d ", visible[i]);
            }
            fprintf(file, "  Cards don't add to 11; start a new pile\n");
        }
        visible = realloc(visible, (numFaceUp + 1) * sizeof(int));
        visible[numFaceUp++] = deck[deckIndexer++];

        if (numFaceUp > 9) {
            gameState = 1;
            break;
        }
    }

    int cardsLeft = 52 - deckIndexer; 

    // manually correcting for when deckInteger overflows
    if (cardsLeft < 0) {
        cardsLeft = 0; 
    } else if (cardsLeft == 0) {
        fprintf(file, "Congratulations you won! %d cards have been played :)\n", deckIndexer);
        fprintf(file, "Cards left in pack: %d\n", cardsLeft);
    } else {
        fprintf(file, "Sorry you lost, %d piles have been created :(\n", numFaceUp);
        fprintf(file, "Cards left in pack: %d\n", cardsLeft);
    }

    if (file) fclose(file);
    free(visible);
    return cardsLeft;
}

int *many_plays(int N) {
    int *remaining = calloc(53, sizeof(int));
    for (int i = 0; i < N; i++) {
        int cardsLeft = play(startDeck, 1);
        remaining[cardsLeft]++;
    }
    return remaining;
}