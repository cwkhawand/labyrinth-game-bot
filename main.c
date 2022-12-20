#include <stdio.h>
#include <stdlib.h>
#include "lib/Labyrinthe/labyrinthAPI.h"
#include "src/Labyrinth.h"

int main() {
    t_labyrinth labyrinth;

    connectToServer("172.105.76.204", 1234, "DataCell");
    waitForLabyrinth("TRAINING DONTMOVE timeout=1000 display=debug", labyrinth.name, &labyrinth.sizeX, &labyrinth.sizeY);

    labyrinth.area = labyrinth.sizeX*labyrinth.sizeY;

    int* temp_labyrinth = malloc(labyrinth.area*5*sizeof(int));

    int myTurn = !getLabyrinth(temp_labyrinth, &labyrinth.extraTile.North, &labyrinth.extraTile.East, &labyrinth.extraTile.South, &labyrinth.extraTile.West, &labyrinth.extraTile.Item);

    initLabyrinth(&labyrinth, temp_labyrinth, myTurn);

    printf("Width: %d   |   Height: %d  |   Name: %s\n\n", labyrinth.sizeX, labyrinth.sizeY, labyrinth.name);

//    printRawLabyrinthDebug(temp_labyrinth, labyrinth.sizeX, labyrinth.sizeY);
//    printf("\n");
//    printLabyrinthDebug(labyrinth);
//    printf("\n\n");

    t_move myMove;
    t_move opponentMove;

    int iWon;
    while (1) {
        printLabyrinth();
        if (myTurn) {
            printf("Please insert the move type, line/column number, rotation and coordinates: ");

            int insert;
            scanf(" %d %d %d %d %d", &insert, &myMove.number, &myMove.rotation, &myMove.x, &myMove.y);
            myMove.insert = (t_insertion) insert;

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

            labyrinth.opponent.x = opponentMove.x;
            labyrinth.opponent.y = opponentMove.y;
        }

        myTurn = !myTurn;
    }

    closeConnection();

    if (iWon) {
        printf("We won!\n");
    } else {
        printf("We lost...\n");
    }

    return 0;
}
