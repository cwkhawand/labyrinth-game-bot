#include <stdio.h>
#include <stdlib.h>
#include "lib/Labyrinth/labyrinthAPI.h"
#include "src/Labyrinth.h"
#include <unistd.h>
#define DEBUG 1

int main() {
    t_labyrinth labyrinth;

    // Connect to server and obtain game information
    connectToServer("172.105.76.204", 3456, "DataCell2");

    int amountOfGames = 1;
    int amountOfWins = 0;
    for (int i = 0; i < amountOfGames; i++) {
        //seed=0x2919b9
        waitForLabyrinth("TRAINING BASIC timeout=1000 display=debug start=0", labyrinth.name, &labyrinth.sizeX, &labyrinth.sizeY);

        labyrinth.area = labyrinth.sizeX * labyrinth.sizeY;

        // allocation enough space for the raw labyrinth
        int *temp_labyrinth = malloc(labyrinth.area * 5 * sizeof(int));

        // obtain labyrinth information
        int my_turn = !getLabyrinth(temp_labyrinth, &labyrinth.extraTile.North, &labyrinth.extraTile.East,
                                    &labyrinth.extraTile.South, &labyrinth.extraTile.West, &labyrinth.extraTile.Item);

        // initiate labyrinth values
        initLabyrinth(&labyrinth, temp_labyrinth, my_turn);

        if (DEBUG)
            printf("Width: %d   |   Height: %d  |   Name: %s\n\n", labyrinth.sizeX, labyrinth.sizeY, labyrinth.name);
        //else
            //printf("Playing against opponent...\n");

        //if (my_turn) printf("I start.\n");
        //else printf("Opponent starts.\n");

        t_move move;

        int i_won;
        int iterations = 0;
        while (1) {
            if (DEBUG)
                printLabyrinth();

            if (my_turn) {
                iterations++;
                /*do {
                    printf("Please insert the move type, line/column number, rotation and coordinates: ");
                    scanf(" %d %d %d %d %d", (int*)&move.insert, &move.number, &move.rotation, &move.x, &move.y);

                    if (move.number % 2 != 1)
                        printf("The line/column number must be an odd number!\n");

                    if (isForbiddenMove(labyrinth, move))
                        printf("This move is forbidden! Please try another one.\n");

                } while(move.number%2 != 1 || isForbiddenMove(labyrinth, move));*/

                t_coordinates item = getItemCoordinates(labyrinth, labyrinth.me.item);
                if (DEBUG)
                    printf("I am at (%d, %d), my item (%d) is at (%d, %d)\n", labyrinth.me.x, labyrinth.me.y,
                           labyrinth.me.item, item.x, item.y);

                move = minimax(labyrinth, move, my_turn, 2);
                if (DEBUG)
                    printf("Move: insert=%d, number=%d, rotation=%d, x=%d, y=%d\n", move.insert, move.number,
                           move.rotation, move.x, move.y);

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

                if (DEBUG)
                    printf("Opponent move: insert=%d, number=%d, rotation=%d, x=%d, y=%d\n", move.insert, move.number,
                           move.rotation, move.x, move.y);

                updateLabyrinth(&labyrinth, move);
            }

            if (DEBUG) {
                printf("I am at (%d, %d)    |   Opponent at (%d, %d)\n", labyrinth.me.x, labyrinth.me.y,
                       labyrinth.opponent.x, labyrinth.opponent.y);
                printf("Extra tile: %d%d%d%d%d\n", labyrinth.extraTile.North, labyrinth.extraTile.East,
                       labyrinth.extraTile.South, labyrinth.extraTile.West, labyrinth.extraTile.Item);
            } else {
                //printf("Next items:\nMe: %d\t\t|\t\tOpponent: %d\n\n", labyrinth.me.item, labyrinth.opponent.item);
            }

            my_turn = !my_turn;
        }

        if (i_won)
            amountOfWins++;

        printf("[Game %d]: %d/%d wins\n", i+1, amountOfWins, i+1);
    }

    printf("We won %.f%% of games!\n", ((float)amountOfWins/(float)amountOfGames)*100.0);

    // Close the connection to the server
    closeConnection();

    /*
    if (i_won) {
        printf("We won in %d iterations!\n", iterations);
    } else {
        printf("We lost...\n");
    }
    */

    return 0;
}
