#ifndef PROJECT_LABYRINTH_H
#define PROJECT_LABYRINTH_H
#include "../lib/Labyrinthe/labyrinthAPI.h"

typedef struct player {
    int x;
    int y;
    int item;
} t_player;

typedef struct tile {
    int North;
    int East;
    int South;
    int West;
    int Item;
} t_tile;

typedef struct labyrinth {
    char name[50];
    int sizeX;
    int sizeY;
    int area;
    t_tile** tiles;
    t_tile extraTile;
    t_player me;
    t_player opponent;
    struct forbiddenMove {
        t_insertion insert;
        int number;
    } forbiddenMove;
} t_labyrinth;

/* Function: printRawLabyrinthDebug
 * Prints the raw labyrinth received from the server.
 * Arguments:
 * - labyrinth: the labyrinth to show
 * - sizeX: the width of the labyrinth
 * - sizeY: the height of the labyrinth
 */
void printRawLabyrinthDebug(int* labyrinth, int sizeX, int sizeY);

/* Function: printRawLabyrinthDebug
 * Prints the parsed labyrinth.
 * Arguments:
 * - labyrinth: the labyrinth to show
 */
void printLabyrinthDebug(t_labyrinth labyrinth);

/* Function: printRawLabyrinthDebug
 * Initiates the labyrinth structure
 * Arguments:
 * - labyrinth: a pointer to the labyrinth structure
 * - temp_labyrinth: the labyrinth raw data received from the server
 * - myTurn: whether it is our turn or the opponent's turn
 */
void initLabyrinth(t_labyrinth* labyrinth, int* temp_labyrinth, int myTurn);

/* Function: updateLabyrinth
 * Updates the labyrinth structure
 * Arguments:
 * - myTurn: whether it was our turn or the opponent's turn
 * - labyrinth: a pointer to the labyrinth structure
 * - move: the move which was done
 */
void updateLabyrinth(t_labyrinth* labyrinth, int myTurn, t_move move);

/* Function: isForbiddenMove
 * Returns 1 if a move is forbidden, 0 otherwise
 * Arguments:
 * - labyrinth: a pointer to the labyrinth structure
 * - move: the move which will be done
 */
int isForbiddenMove(t_labyrinth labyrinth, t_move move);

#endif
