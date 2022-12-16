#include <stdio.h>
#include "Labyrinth.h"

void printLabyrinthDebug(int* labyrinth, int sizeX, int sizeY) {
    for (int i = 0; i < sizeX; i++) {
        for (int j = 0; j < sizeY; j++) {
            for (int k = 0; k < 5; k++) {
                printf("%d", labyrinth[i*j + k]);
            }
            printf(" ");
        }
        printf("\n");
    }
}