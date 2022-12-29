#include <stdio.h>
#include <stdlib.h>
#include "lib/Labyrinthe/labyrinthAPI.h"
#include "src/Labyrinth.h"

int main() {
    t_labyrinth labyrinth;

    // Connect to server and obtain game information
    connectToServer("172.105.76.204", 3456, "DataCell");
    waitForLabyrinth("TRAINING RANDOM timeout=1000 start=0 seed=0x02a1da", labyrinth.name, &labyrinth.sizeX, &labyrinth.sizeY);

    labyrinth.area = labyrinth.sizeX*labyrinth.sizeY;

    // allocation enough space for the raw labyrinth
    int* temp_labyrinth = malloc(labyrinth.area*5*sizeof(int));

    // obtain labyrinth information
    int my_turn = !getLabyrinth(temp_labyrinth, &labyrinth.extraTile.North, &labyrinth.extraTile.East, &labyrinth.extraTile.South, &labyrinth.extraTile.West, &labyrinth.extraTile.Item);

    // initiate labyrinth values
    initLabyrinth(&labyrinth, temp_labyrinth, my_turn);

    printf("Width: %d   |   Height: %d  |   Name: %s\n\n", labyrinth.sizeX, labyrinth.sizeY, labyrinth.name);

    t_move move;

    int i_won;
    while (1) {
        printLabyrinth();

//        printLabyrinthDebug(labyrinth);

        if (my_turn) {
            do {
                printf("Please insert the move type, line/column number, rotation and coordinates: ");
                scanf(" %d %d %d %d %d", (int*)&move.insert, &move.number, &move.rotation, &move.x, &move.y);

                if (move.number % 2 != 1)
                    printf("The line/column number must be an odd number!\n");

                if (isForbiddenMove(labyrinth, move))
                    printf("This move is forbidden! Please try another one.\n");

            } while(move.number%2 != 1 || isForbiddenMove(labyrinth, move));

            int moveCode = sendMove(&move);

            if (moveCode != NORMAL_MOVE) {
                if (moveCode == WINNING_MOVE) i_won = 1;
                else i_won = 0;
                break;
            }

            playMyTurn(&labyrinth, move);
        } else {
            int moveCode = getMove(&move);
            if (moveCode != NORMAL_MOVE) {
                if (moveCode == WINNING_MOVE) i_won = 0;
                else i_won = 1;
                break;
            }

            updateLabyrinth(&labyrinth, move);
        }

        printf("I am at (%d, %d)    |   Opponent at (%d, %d)\n", labyrinth.me.x, labyrinth.me.y, labyrinth.opponent.x, labyrinth.opponent.y);
        my_turn = !my_turn;
    }

    // Close the connection to the server
    closeConnection();

    if (i_won) {
        printf("We won!\n");
    } else {
        printf("We lost...\n");
    }

    return 0;
}
