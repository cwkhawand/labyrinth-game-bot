#include <stdio.h>
#include <stdlib.h>
#include "Labyrinth.h"

/* Function: printRawLabyrinthDebug
 * Prints the raw labyrinth received from the server.
 * Arguments:
 * - labyrinth: the labyrinth to show
 * - sizeX: the width of the labyrinth
 * - sizeY: the height of the labyrinth
 */
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

/* Function: printRawLabyrinthDebug
 * Prints the parsed labyrinth.
 * Arguments:
 * - labyrinth: the labyrinth to show
 */
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

/* Function: printRawLabyrinthDebug
 * Initiates the labyrinth structure
 * Arguments:
 * - labyrinth: a pointer to the labyrinth structure
 * - temp_labyrinth: the labyrinth raw data received from the server
 * - myTurn: whether it is our turn or the opponent's turn
 */
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

/* Function: updateLabyrinth
 * Updates the labyrinth structure
 * Arguments:
 * - labyrinth: a pointer to the labyrinth structure
 * - myTurn: whether it was our turn or the opponent's turn
 * - move: the move which was done
 */
void updateLabyrinth(t_labyrinth* labyrinth, int myTurn, t_move move) {
    // Update our or the opponent's position and item
    if (myTurn) {
        labyrinth->me.x = move.x;
        labyrinth->me.y = move.y;
        labyrinth->me.item = move.nextItem;
    } else {
        labyrinth->opponent.x = move.x;
        labyrinth->opponent.y = move.y;
        labyrinth->opponent.item = move.nextItem;
    }

    // move lines/columns according to the inserted tile
    if (move.insert == INSERT_LINE_LEFT) {
        for (int i = labyrinth->sizeX-2; i >= 0; i--) {
            labyrinth->tiles[move.number*labyrinth->sizeX + i+1] = labyrinth->tiles[move.number*labyrinth->sizeX + i];
        }
        labyrinth->tiles[move.number*labyrinth->sizeX] = labyrinth->extraTile;

    } else if (move.insert == INSERT_LINE_RIGHT) {
        for (int i = 1; i < labyrinth->sizeX; i++) {
            labyrinth->tiles[move.number*labyrinth->sizeX + i-1] = labyrinth->tiles[move.number*labyrinth->sizeX + i];
        }
        labyrinth->tiles[move.number*labyrinth->sizeX + labyrinth->sizeX - 1] = labyrinth->extraTile;
    } else if (move.insert == INSERT_COLUMN_TOP) {
        for (int i = labyrinth->sizeY-2; i >= 0; i--) {
            labyrinth->tiles[(i+1)*labyrinth->sizeX + move.number] = labyrinth->tiles[i*labyrinth->sizeX + move.number];
        }
        labyrinth->tiles[move.number] = labyrinth->extraTile;
    } else if (move.insert == INSERT_COLUMN_BOTTOM) {
        for (int i = 1; i < labyrinth->sizeY; i++) {
            labyrinth->tiles[(i-1)*labyrinth->sizeX + move.number] = labyrinth->tiles[i*labyrinth->sizeX + move.number];
        }
        labyrinth->tiles[(labyrinth->sizeY-1)*labyrinth->sizeX + move.number] = labyrinth->extraTile;
    }

    // Update the extra tile
    labyrinth->extraTile.North = move.tileN;
    labyrinth->extraTile.East = move.tileE;
    labyrinth->extraTile.South = move.tileS;
    labyrinth->extraTile.West = move.tileW;
    labyrinth->extraTile.Item = move.tileItem;
}