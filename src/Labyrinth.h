#ifndef PROJECT_LABYRINTH_H
#define PROJECT_LABYRINTH_H

typedef struct pos {
    int x;
    int y;
} t_pos;

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
    t_tile* tiles;
    t_tile extraTile;
    t_pos me;
    t_pos opponent;
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

#endif
