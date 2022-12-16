#include <stdio.h>
#include <stdlib.h>
#include "lib/Labyrinthe/labyrinthAPI.h"

typedef struct game {
    char name[50];
    int sizeX;
    int sizeY;
} t_game;

typedef struct tile {
    int North;
    int East;
    int South;
    int West;
    int Item;
} t_tile;

int main() {
    printf("Connecting to server...\n");
    connectToServer("172.105.76.204", 1234, "DataCell");
    printf("Connected to server, obtaining labyrinth...\n");

    t_game game;
    waitForLabyrinth("TRAINING DONTMOVE timeout=1000", game.name, &game.sizeX, &game.sizeY);

    int* labyrinth = malloc(game.sizeX*game.sizeY*5*sizeof(int));
    t_tile externalTile;
    int myTurn = !getLabyrinth(labyrinth, &externalTile.North, &externalTile.East, &externalTile.South, &externalTile.West, &externalTile.Item);
    printf("Obtained labyrinth from server\n");

    int iWon;
    t_move opponentMove;
    while (1) {
        printLabyrinth();
        if (myTurn) {
            t_move myMove;
            printf("Please insert the move type, line/column number, rotation and coordinates: ");
            scanf(" %d %d %d %d %d", &myMove.insert, &myMove.number, &myMove.rotation, &myMove.x, &myMove.y);
            int moveCode = sendMove(&myMove);

            if (moveCode != NORMAL_MOVE) {
                if (moveCode == WINNING_MOVE) iWon = 1;
                else iWon = 0;
                break;
            }
        } else {
            int moveCode = getMove(&opponentMove);
            if (moveCode != NORMAL_MOVE) {
                if (moveCode == WINNING_MOVE) iWon = 0;
                else iWon = 1;
                break;
            }
        }
    }

    closeConnection();

    if (iWon) {
        printf("We won!\n");
    } else {
        printf("We lost...\n");
    }

    return 0;
}
