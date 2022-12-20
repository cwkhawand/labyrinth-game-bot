#include <stdio.h>
#include <stdlib.h>
#include "Labyrinth.h"
#include "../lib/Labyrinthe/labyrinthAPI.h"

void printRawLabyrinthDebug(int* labyrinth, int sizeX, int sizeY) {
    for (int i = 0; i < sizeX*sizeY*5; i++) {
        printf("%d", labyrinth[i]);
    }
    printf("\n\n");
    for (int i = 0; i < sizeY; i++) {
        for (int j = 0; j < sizeX; j++) {
            for (int k = 0; k < 5; k++) {
                printf("%d", labyrinth[i*sizeX*5 + j*5 + k]);
            }
            printf(" ");
        }
        printf("\n");
    }
}

void printLabyrinthDebug(t_labyrinth labyrinth) {
    for (int i = 0; i < labyrinth.sizeY; i++) {
        for (int j = 0; j < labyrinth.sizeX; j++) {
                printf("%d%d%d%d%d ",
                       labyrinth.tiles[i*labyrinth.sizeX + j].North,
                       labyrinth.tiles[i*labyrinth.sizeX + j].East,
                       labyrinth.tiles[i*labyrinth.sizeX + j].South,
                       labyrinth.tiles[i*labyrinth.sizeX + j].West,
                       labyrinth.tiles[i*labyrinth.sizeX + j].Item
                );
        }
        printf("\n");
    }
}

void initLabyrinth(t_labyrinth* labyrinth, int* temp_labyrinth, int myTurn) {
    labyrinth->me.x = (myTurn) ? 0 : labyrinth->sizeX-1;
    labyrinth->me.y = (myTurn) ? 0 : labyrinth->sizeY-1;

    labyrinth->opponent.x = (myTurn) ? labyrinth->sizeX-1 : 0;
    labyrinth->opponent.y = (myTurn) ? labyrinth->sizeY-1 : 0;

    labyrinth->tiles = (t_tile*)malloc(labyrinth->area*sizeof(t_tile));

    for (int i = 0; i < labyrinth->sizeY; i++) {
        for (int j = 0; j < labyrinth->sizeX; j++) {
            labyrinth->tiles[i*labyrinth->sizeX + j].North = temp_labyrinth[i*labyrinth->sizeX*5 + j*5];
            labyrinth->tiles[i*labyrinth->sizeX + j].East = temp_labyrinth[i*labyrinth->sizeX*5 + j*5 + 1];
            labyrinth->tiles[i*labyrinth->sizeX + j].South = temp_labyrinth[i*labyrinth->sizeX*5 + j*5 + 2];
            labyrinth->tiles[i*labyrinth->sizeX + j].West = temp_labyrinth[i*labyrinth->sizeX*5 + j*5 + 3];
            labyrinth->tiles[i*labyrinth->sizeX + j].Item = temp_labyrinth[i*labyrinth->sizeX*5 + j*5 + 4];
        }
    }
}