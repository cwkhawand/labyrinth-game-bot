#ifndef PROJECT_LABYRINTH_H
#define PROJECT_LABYRINTH_H
#include "../lib/Labyrinth/labyrinthAPI.h"
#include <limits.h>
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct coordinates {
    int x;
    int y;
    int distance; // optional distance field
} t_coordinates;

typedef struct player {
    int x;
    int y;
    int item;
    int startingItem;
    int finishingItem;
    int starts;
} t_player;

typedef struct tile {
    int North;
    int East;
    int South;
    int West;
    int Item;
    int isVisited;
} t_tile;

typedef struct node t_node;
struct node {
    t_move head;
    int score;
    int initialized;
    t_node* children;
};

struct movesByDistance {
    t_move move;
    t_coordinates tile;
    int initialized;
    int allowsOpponentToReach;
};

typedef struct labyrinth {
    char name[50];
    int sizeX;
    int sizeY;
    int area;
    int amountOfPossibleMoves;
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

/* Function: rotateTile
 * Inserts a tile into the labyrinth
 * Arguments:
 * - tile: a pointer to the tile to be rotated
 * - rotations: the amount of quarter clockwise rotations to be done (0 to 3)
 */
void rotateTile(t_tile* tile, int rotations);

/* Function: movePlayer
 * Moves a given player according to the insertion
 * - labyrinth: the labyrinth structure
 * - player: a pointer to the player
 * - move: the move which was done
 */
void movePlayer(t_labyrinth labyrinth, t_player* player, t_move move);

/* Function: insertExtraTile
 * Inserts a tile into the labyrinth
 * Arguments:
 * - labyrinth: a pointer to the labyrinth structure
 * - move: the move which was done
 */
void insertExtraTile(t_labyrinth* labyrinth, t_move move);

/* Function: playMyTurn
 * Updates the labyrinth structure when we make a move
 * Arguments:
 * - labyrinth: a pointer to the labyrinth structure
 * - move: the move which was done
 */
void playMyTurn(t_labyrinth* labyrinth, t_move move);

/* Function: updateLabyrinth
 * Updates the labyrinth structure when the opponent makes a move
 * Arguments:
 * - labyrinth: a pointer to the labyrinth structure
 * - move: the move which was done
 */
void updateLabyrinth(t_labyrinth* labyrinth, t_move move);

/* Function: isForbiddenMove
 * Returns 1 if a move is forbidden, 0 otherwise
 * Arguments:
 * - labyrinth: a pointer to the labyrinth structure
 * - move: the move which will be done
 */
int isForbiddenMove(t_labyrinth labyrinth, t_move move);

/* Function: copyLabyrinth
 * Deep copies a labyrinth
 * Arguments:
 * - labyrinth: the labyrinth structure
 * - labyrinth_copy: a pointer to an initialized t_labyrinth structure
 */
void copyLabyrinth(t_labyrinth labyrinth, t_labyrinth* labyrinth_copy);

/* Function: copyLabyrinth
 * Deep frees a labyrinth from memory
 * Arguments:
 * - labyrinth: a pointer to the labyrinth structure
 */
void freeLabyrinth(t_labyrinth* labyrinth);

/* Function: getItemCoordinates
 * Returns the coordinates of a given item
 * Arguments:
 * - labyrinth: a labyrinth structure
 * - item: the number of the item
 */
t_coordinates getItemCoordinates(t_labyrinth labyrinth, int item);

/* Function: updateMovesByDistance
 * Inserts a move into a moves list sorted by distance to destination
 * Arguments:
 * - moves: the moves list sorted by distance
 * - amountOfPossibleMoves: the size of moves
 * - move: the move to be inserted
 * - tile: the closest tile to the destination corresponding to the move
 */
void updateMovesByDistance(struct movesByDistance* moves, int amountOfPossibleMoves, t_move move, t_coordinates tile, int allowsOpponentToReach);

/* Function: getNeighbour
 * Returns the neighbour of a given tile
 * Arguments:
 * - labyrinth: a labyrinth structure
 * - tile: the coordinates of the source tile
 * - direction: the direction in which to move (0 to 3 resp: North, East, South, West)
 */
t_coordinates getNeighbour(t_labyrinth labyrinth, t_coordinates tile, int direction);

/* Function: isReachableOtherwiseClosest
 * Returns 1 if the destination is reachable from the source, 0 otherwise.
 * Also fills closestTile with the closest tile to the destination.
 * Arguments:
 * - labyrinth: a labyrinth structure
 * - source: starting coordinates
 * - destination: ending coordinates
 * - currentTile: the current tile we're expanding from. The source tile if calling the function
 * - closestTile: a pointer to the closest tile. The source with its distance if calling the function
 */
int isReachableOtherwiseClosest(t_labyrinth labyrinth, t_coordinates source, t_coordinates destination, t_coordinates currentTile, t_coordinates* closestTile);

/* Function: findBestMove
 * Attempt to find the best move to be played
 * Arguments:
 * - labyrinth: a labyrinth structure
 */
t_move findBestMove(t_labyrinth labyrinth);

int buildMinimaxGraph (t_labyrinth labyrinth, t_node* node, int maximizingPlayer, int depth, int maxDepth);

void freeMinimaxGraph(t_labyrinth labyrinth, t_node node);

t_move minimax (t_labyrinth labyrinth, t_move move, int myTurn, int maxDepth);
#endif
