#include <stdio.h>
#include <stdlib.h>
#include "lib/Labyrinthe/labyrinthAPI.h"
#include "src/Labyrinth.h"

int main() {
    t_labyrinth labyrinth;

    // Connect to server and obtain game information
    connectToServer("172.105.76.204", 1234, "DataCell");
    waitForLabyrinth("TRAINING DONTMOVE timeout=1000 start=0 seed=0x02a1da", labyrinth.name, &labyrinth.sizeX, &labyrinth.sizeY);

    labyrinth.area = labyrinth.sizeX*labyrinth.sizeY;

    // allocation enough space for the raw labyrinth
    int* temp_labyrinth = malloc(labyrinth.area*5*sizeof(int));

    // obtain labyrinth information
    int myTurn = !getLabyrinth(temp_labyrinth, &labyrinth.extraTile.North, &labyrinth.extraTile.East, &labyrinth.extraTile.South, &labyrinth.extraTile.West, &labyrinth.extraTile.Item);

    // initiate labyrinth values
    initLabyrinth(&labyrinth, temp_labyrinth, myTurn);

    printf("Width: %d   |   Height: %d  |   Name: %s\n\n", labyrinth.sizeX, labyrinth.sizeY, labyrinth.name);

    t_move move;

    int iWon;
    while (1) {
        printLabyrinth();

//        printLabyrinthDebug(labyrinth);

        if (myTurn) {
            int insert;
            do {
                printf("Please insert the move type, line/column number, rotation and coordinates: ");
                scanf(" %d %d %d %d %d", &insert, &move.number, &move.rotation, &move.x, &move.y);
                move.insert = (t_insertion) insert;

                if (move.number % 2 != 1)
                    printf("The line/column number must be an odd number!\n");

                if (isForbiddenMove(labyrinth, move))
                    printf("This move is forbidden! Please try another one.\n");

            } while(move.number%2 != 1 || isForbiddenMove(labyrinth, move));

            int moveCode = sendMove(&move);

            if (moveCode != NORMAL_MOVE) {
                if (moveCode == WINNING_MOVE) iWon = 1;
                else iWon = 0;
                break;
            }


        } else {
            int moveCode = getMove(&move);
            if (moveCode != NORMAL_MOVE) {
                if (moveCode == WINNING_MOVE) iWon = 0;
                else iWon = 1;
                break;
            }

            labyrinth.opponent.x = move.x;
            labyrinth.opponent.y = move.y;
        }

        updateLabyrinth(&labyrinth, myTurn, move);

        myTurn = !myTurn;
    }

    // Close the connection to the server
    closeConnection();

    if (iWon) {
        printf("We won!\n");
    } else {
        printf("We lost...\n");
    }

    return 0;
}
