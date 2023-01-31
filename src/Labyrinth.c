#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
                       labyrinth.tiles[i][j].North,
                       labyrinth.tiles[i][j].East,
                       labyrinth.tiles[i][j].South,
                       labyrinth.tiles[i][j].West,
                       labyrinth.tiles[i][j].Item
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
    // set our initial position according to who starts
    labyrinth->me.x = (myTurn) ? 0 : labyrinth->sizeX-1;
    labyrinth->me.y = (myTurn) ? 0 : labyrinth->sizeY-1;
    labyrinth->me.item = (myTurn) ? 1 : 24;
    labyrinth->me.startingItem = (myTurn) ? 1 : 24;
    labyrinth->me.finishingItem = (myTurn) ? 24 : 1;
    labyrinth->me.starts = myTurn;

    // set opponent's initial position according to who starts
    labyrinth->opponent.x = (myTurn) ? labyrinth->sizeX-1 : 0;
    labyrinth->opponent.y = (myTurn) ? labyrinth->sizeY-1 : 0;
    labyrinth->opponent.item = (myTurn) ? 24 : 1;
    labyrinth->opponent.startingItem = (myTurn) ? 24 : 1;
    labyrinth->opponent.finishingItem = (myTurn) ? 1 : 24;
    labyrinth->opponent.starts = !myTurn;

    // set forbidden move to not existant
    labyrinth->forbiddenMove.insert = -1;
    labyrinth->forbiddenMove.number = 0;

    labyrinth->amountOfPossibleMoves = (int)(ceil(((double)labyrinth->sizeX)/2) + ceil(((double)labyrinth->sizeY)/2))*8;

    labyrinth->extraTile.isVisited = 0;

    // allocate a 2 dimensional array for our labyrinth
    labyrinth->tiles = (t_tile**)malloc(labyrinth->sizeY*sizeof(t_tile*));
    for (int i = 0; i < labyrinth->sizeY; i++) {
        labyrinth->tiles[i] = (t_tile*)malloc(labyrinth->sizeX*sizeof(t_tile));
    }

    for (int i = 0; i < labyrinth->sizeY; i++) {
        for (int j = 0; j < labyrinth->sizeX; j++) {
            labyrinth->tiles[i][j].North = temp_labyrinth[i*labyrinth->sizeX*5 + j*5];
            labyrinth->tiles[i][j].East = temp_labyrinth[i*labyrinth->sizeX*5 + j*5 + 1];
            labyrinth->tiles[i][j].South = temp_labyrinth[i*labyrinth->sizeX*5 + j*5 + 2];
            labyrinth->tiles[i][j].West = temp_labyrinth[i*labyrinth->sizeX*5 + j*5 + 3];
            labyrinth->tiles[i][j].Item = temp_labyrinth[i*labyrinth->sizeX*5 + j*5 + 4];
            labyrinth->tiles[i][j].isVisited = 0;
        }
    }
}

/* Function: rotateTile
 * Inserts a tile into the labyrinth
 * Arguments:
 * - tile: the tile to be rotated
 * - rotations: the amount of quarter clockwise rotations to be done (0 to 3)
 */
void rotateTile(t_tile* tile, int rotations) {
    for (int i = 0; i < rotations; i++) {
        int North = tile->West;
        int East = tile->North;
        int South = tile->East;
        int West = tile->South;
        tile->North = North;
        tile->East = East;
        tile->South = South;
        tile->West = West;
    }
}

/* Function: movePlayer
 * Moves a given player according to the insertion
 * - labyrinth: the labyrinth structure
 * - player: a pointer to the player
 * - move: the move which was done
 */
void movePlayer(t_labyrinth labyrinth, t_player* player, t_move move) {
    if (move.insert == INSERT_LINE_LEFT && player->y == move.number) {
        player->x = (player->x+1)%labyrinth.sizeX;
    } else if (move.insert == INSERT_LINE_RIGHT && player->y == move.number) {
        player->x = (player->x-1)%labyrinth.sizeX;
        if (player->x < 0) player->x = labyrinth.sizeX-1;
    } else if (move.insert == INSERT_COLUMN_TOP && player->x == move.number) {
        player->y = (player->y+1)%labyrinth.sizeY;
    } else if (move.insert == INSERT_COLUMN_BOTTOM && player->x == move.number) {
        player->y = (player->y-1)%labyrinth.sizeY;
        if (player->y < 0) player->y = labyrinth.sizeY-1;
    }
}

/* Function: insertExtraTile
 * Inserts a tile into the labyrinth
 * Arguments:
 * - labyrinth: a pointer to the labyrinth structure
 * - move: the move which was done
 */
void insertExtraTile(t_labyrinth* labyrinth, t_move move) {
    rotateTile(&labyrinth->extraTile, move.rotation);

    t_tile extraTile;

    // move lines/columns according to the inserted tile
    if (move.insert == INSERT_LINE_LEFT) {
        extraTile = labyrinth->tiles[move.number][labyrinth->sizeX-1];
        for (int i = labyrinth->sizeX-2; i >= 0; i--) {
            labyrinth->tiles[move.number][i+1] = labyrinth->tiles[move.number][i];
        }
        labyrinth->tiles[move.number][0] = labyrinth->extraTile;

    } else if (move.insert == INSERT_LINE_RIGHT) {
        extraTile = labyrinth->tiles[move.number][0];
        for (int i = 1; i < labyrinth->sizeX; i++) {
            labyrinth->tiles[move.number][i-1] = labyrinth->tiles[move.number][i];
        }
        labyrinth->tiles[move.number][labyrinth->sizeX - 1] = labyrinth->extraTile;
    } else if (move.insert == INSERT_COLUMN_TOP) {
        extraTile = labyrinth->tiles[labyrinth->sizeY-1][move.number];
        for (int i = labyrinth->sizeY-2; i >= 0; i--) {
            labyrinth->tiles[i+1][move.number] = labyrinth->tiles[i][move.number];
        }
        labyrinth->tiles[0][move.number] = labyrinth->extraTile;
    } else if (move.insert == INSERT_COLUMN_BOTTOM) {
        extraTile = labyrinth->tiles[0][move.number];
        for (int i = 1; i < labyrinth->sizeY; i++) {
            labyrinth->tiles[i-1][move.number] = labyrinth->tiles[i][move.number];
        }
        labyrinth->tiles[labyrinth->sizeY-1][move.number] = labyrinth->extraTile;
    }

    labyrinth->extraTile = extraTile;
    // Update the forbidden move
    int oppositeMoves[4] = {1, 0, 3, 2};
    labyrinth->forbiddenMove.insert = oppositeMoves[move.insert];

    labyrinth->forbiddenMove.number = move.number;
}

/* Function: playMyTurn
 * Updates the labyrinth structure when we make a move
 * Arguments:
 * - labyrinth: a pointer to the labyrinth structure
 * - move: the move which was done
 */
void playMyTurn(t_labyrinth* labyrinth, t_move move) {
    insertExtraTile(labyrinth, move);

    // Update players positions and items
    labyrinth->me.x = move.x;
    labyrinth->me.y = move.y;
    labyrinth->me.item = move.nextItem;

    movePlayer(*labyrinth, &labyrinth->opponent, move);

    // Update the extra tile
//    labyrinth->extraTile.North = move.tileN;
//    labyrinth->extraTile.East = move.tileE;
//    labyrinth->extraTile.South = move.tileS;
//    labyrinth->extraTile.West = move.tileW;
//    labyrinth->extraTile.Item = move.tileItem;
}

/* Function: updateLabyrinth
 * Updates the labyrinth structure when the opponent makes a move
 * Arguments:
 * - labyrinth: a pointer to the labyrinth structure
 * - move: the move which was done
 */
void updateLabyrinth(t_labyrinth* labyrinth, t_move move) {
    insertExtraTile(labyrinth, move);

    // Update players positions and items
    movePlayer(*labyrinth, &labyrinth->me, move);
    labyrinth->opponent.x = move.x;
    labyrinth->opponent.y = move.y;
    labyrinth->opponent.item = move.nextItem;

    // Update the extra tile
//    labyrinth->extraTile.North = move.tileN;
//    labyrinth->extraTile.East = move.tileE;
//    labyrinth->extraTile.South = move.tileS;
//    labyrinth->extraTile.West = move.tileW;
//    labyrinth->extraTile.Item = move.tileItem;
}

/* Function: isForbiddenMove
 * Returns 1 if a move is forbidden, 0 otherwise
 * Arguments:
 * - labyrinth: a pointer to the labyrinth structure
 * - move: the move which will be done
 */
int isForbiddenMove(t_labyrinth labyrinth, t_move move) {
    if (labyrinth.forbiddenMove.insert == move.insert && labyrinth.forbiddenMove.number == move.number)
        return 1;

    return 0;
}

/* Function: copyLabyrinth
 * Deep copies a labyrinth
 * Arguments:
 * - labyrinth: the labyrinth structure
 * - labyrinth_copy: a pointer to an initialized t_labyrinth structure
 */
void copyLabyrinth(t_labyrinth labyrinth, t_labyrinth* labyrinth_copy) {
    *labyrinth_copy = labyrinth; // copy everything that's not a pointer

    // allocate a 2 dimensional array for our labyrinth copy
    labyrinth_copy->tiles = (t_tile**)malloc(labyrinth_copy->sizeY*sizeof(t_tile*));
    for (int i = 0; i < labyrinth_copy->sizeY; i++) {
        labyrinth_copy->tiles[i] = (t_tile*)malloc(labyrinth_copy->sizeX*sizeof(t_tile));
    }

    for (int i = 0; i < labyrinth.sizeY; i++) {
        for (int j = 0; j < labyrinth.sizeX; j++) {
            labyrinth_copy->tiles[i][j] = labyrinth.tiles[i][j];
        }
    }
}

/* Function: copyLabyrinth
 * Deep frees a labyrinth from memory
 * Arguments:
 * - labyrinth: a pointer to the labyrinth structure
 */
void freeLabyrinth(t_labyrinth* labyrinth) {
    // de-allocate the 2 dimensional array of our labyrinth copy
    for (int i = 0; i < labyrinth->sizeY; i++) {
        free(labyrinth->tiles[i]);
    }
    free(labyrinth->tiles);
}

/* Function: getItemCoordinates
 * Returns the coordinates of a given item
 * Arguments:
 * - labyrinth: a labyrinth structure
 * - item: the number of the item
 */
t_coordinates getItemCoordinates(t_labyrinth labyrinth, int item) {
    // Attempt to find item in maze
    for (int i = 0; i < labyrinth.sizeY; i++) {
        for (int j = 0; j < labyrinth.sizeX; j++) {
            if (labyrinth.tiles[i][j].Item == item) {
                t_coordinates coordinates = { .x = j, .y = i };
                return coordinates;
            }
        }
    }

    // Otherwise it's in the extra tile
    t_coordinates coordinates = { .x = -1, .y = -1};
    return coordinates;
}

/* Function: updateMovesByDistance
 * Inserts a move into a moves list sorted by distance to destination
 * Arguments:
 * - moves: the moves list sorted by distance
 * - amountOfPossibleMoves: the size of moves
 * - move: the move to be inserted
 * - tile: the closest tile to the destination corresponding to the move
 */
void updateMovesByDistance(struct movesByDistance* moves, int amountOfPossibleMoves, t_move move, t_coordinates tile, int allowsOpponentToReach) {
    int insertAt = -1;

    for (int i = 0; i < amountOfPossibleMoves; i++) {
        if (moves[i].initialized == 0 || tile.distance < moves[i].tile.distance) {
            insertAt = i;
            break;
        }
    }

    for (int i = amountOfPossibleMoves-2; i >= insertAt; i--) {
        if (moves[i].initialized != 0)
            moves[i + 1] = moves[i];
    }

    moves[insertAt].move = move;
    moves[insertAt].tile = tile;
    moves[insertAt].initialized = 1;
    moves[insertAt].allowsOpponentToReach = allowsOpponentToReach;
}

/* Function: getNeighbour
 * Returns the neighbour of a given tile
 * Arguments:
 * - labyrinth: a labyrinth structure
 * - tile: the coordinates of the source tile
 * - direction: the direction in which to move (0 to 3 resp: North, East, South, West)
 */
t_coordinates getNeighbour(t_labyrinth labyrinth, t_coordinates tile, int direction) {
    t_coordinates neighbour = { .x = tile.x, .y = tile.y };

    /* We modify x or y according to the direction */
    if (direction == 0) neighbour.y = neighbour.y-1;
    if (direction == 1) neighbour.x = neighbour.x+1;
    if (direction == 2) neighbour.y = neighbour.y+1;
    if (direction == 3) neighbour.x = neighbour.x-1;

    if (neighbour.x < 0 || neighbour.y < 0 || neighbour.x >= labyrinth.sizeX || neighbour.y >= labyrinth.sizeY) {
        neighbour.x = -1;
        neighbour.y = -1;
    }

    return neighbour;
}

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
int isReachableOtherwiseClosest(t_labyrinth labyrinth, t_coordinates source, t_coordinates destination, t_coordinates currentTile, t_coordinates* closestTile) {
    t_tile** maze = labyrinth.tiles;

    maze[source.y][source.x].isVisited = 1; // source tile is always at distance 1

    if (maze[currentTile.y][currentTile.x].isVisited == 0) return 0; // skip non-visited tiles

    // Find the closest discovered point to destination
    int distanceFromDestination = abs(currentTile.x - destination.x) + abs(currentTile.y - destination.y);
    if (distanceFromDestination < closestTile->distance) {
        closestTile->x = currentTile.x;
        closestTile->y = currentTile.y;
        closestTile->distance = distanceFromDestination;
    }

    if (currentTile.x == destination.x && currentTile.y == destination.y) return 1; // when we reach the destination, exit

    // We try expanding from each neighbour
    for (int direction = 0; direction < 4; direction++) {
        // Skip walls
        if (direction == 0 && maze[currentTile.y][currentTile.x].North == 1) continue;
        if (direction == 1 && maze[currentTile.y][currentTile.x].East == 1) continue;
        if (direction == 2 && maze[currentTile.y][currentTile.x].South == 1) continue;
        if (direction == 3 && maze[currentTile.y][currentTile.x].West == 1) continue;

        t_coordinates neighbour = getNeighbour(labyrinth, currentTile, direction);
        // If neighbour is outside the maze, skip it
        if (neighbour.x == -1 || neighbour.y == -1) continue;

        // can we go back to our original tile from the neighbour?
        if (direction == 0 && maze[neighbour.y][neighbour.x].South == 1) continue;
        if (direction == 1 && maze[neighbour.y][neighbour.x].West == 1) continue;
        if (direction == 2 && maze[neighbour.y][neighbour.x].North == 1) continue;
        if (direction == 3 && maze[neighbour.y][neighbour.x].East == 1) continue;

        if (maze[neighbour.y][neighbour.x].isVisited == 0) {
            maze[neighbour.y][neighbour.x].isVisited = maze[currentTile.y][currentTile.x].isVisited + 1;
            if (isReachableOtherwiseClosest(labyrinth, source, destination, neighbour, closestTile))
                return 1;
        }
    }

    return 0;
}

int allowsOpponentToReach(t_labyrinth labyrinth){
    int correspondingSize[4] = {labyrinth.sizeY-1, labyrinth.sizeY-1, labyrinth.sizeX-1, labyrinth.sizeX-1};
    t_move move;
    for (int insert = 0; insert < 4; insert++) {
        for (int number = 1; number < correspondingSize[insert]; number += 2) {
            if (labyrinth.forbiddenMove.insert == insert && labyrinth.forbiddenMove.number == number) continue;

            for (int rotation = 0; rotation < 4; rotation++) {
                t_labyrinth labyrinth_copy;
                copyLabyrinth(labyrinth, &labyrinth_copy);

                move.insert = insert;
                move.number = number;
                move.rotation = rotation;
                insertExtraTile(&labyrinth_copy, move);

                // Update players positions and items
                movePlayer(labyrinth_copy, &labyrinth_copy.me, move);
                movePlayer(labyrinth_copy, &labyrinth_copy.opponent, move);

                t_coordinates destination = getItemCoordinates(labyrinth_copy, labyrinth_copy.opponent.item);

                // If item is ejected, skip the move
                if (destination.x == -1 && destination.y == -1) {
                    freeLabyrinth(&labyrinth_copy);
                    continue;
                }

                t_coordinates source = { .x = labyrinth_copy.opponent.x, .y = labyrinth_copy.opponent.y, .distance = abs(labyrinth_copy.opponent.x - destination.x) + abs(labyrinth_copy.opponent.y - destination.y) };
                t_coordinates closestTile = source;
                int allowsReaching = 0;
                if (isReachableOtherwiseClosest(labyrinth_copy, source, destination, source, &closestTile))
                    allowsReaching = 1;

                freeLabyrinth(&labyrinth_copy);

                if (allowsReaching)
                    return 1;
            }
        }
    }

    return 0;
}

/* Function: findBestMove
 * Attempt to find the best move to be played
 * Arguments:
 * - labyrinth: a labyrinth structure
 */
t_move findBestMove(t_labyrinth labyrinth) {
    struct movesByDistance* moves = calloc(labyrinth.amountOfPossibleMoves, sizeof(struct movesByDistance));

    int correspondingSize[4] = {labyrinth.sizeY-1, labyrinth.sizeY-1, labyrinth.sizeX-1, labyrinth.sizeX-1};
    int winningMoves = 0;
    t_move move;
    for (int insert = 0; insert < 4 && !winningMoves; insert++) {
        for (int number = 1; number < correspondingSize[insert] && !winningMoves; number += 2) {
            if (labyrinth.forbiddenMove.insert == insert && labyrinth.forbiddenMove.number == number) continue;

            for (int rotation = 0; rotation < 4; rotation++) {
                t_labyrinth labyrinth_copy;
                copyLabyrinth(labyrinth, &labyrinth_copy);

                move.insert = insert;
                move.number = number;
                move.rotation = rotation;
                insertExtraTile(&labyrinth_copy, move);

                // Update players positions and items
                movePlayer(labyrinth_copy, &labyrinth_copy.me, move);
                movePlayer(labyrinth_copy, &labyrinth_copy.opponent, move);

                t_coordinates destination = getItemCoordinates(labyrinth_copy, labyrinth_copy.me.item);

                // If item is ejected, skip the move
                if (destination.x == -1 && destination.y == -1) {
                    freeLabyrinth(&labyrinth_copy);
                    continue;
                }

                t_coordinates source = { .x = labyrinth_copy.me.x, .y = labyrinth_copy.me.y, .distance = abs(labyrinth_copy.me.x - destination.x) + abs(labyrinth_copy.me.y - destination.y) };
                t_coordinates closestTile = source;
                if (isReachableOtherwiseClosest(labyrinth_copy, source, destination, source, &closestTile))
                    winningMoves += 1;

                updateMovesByDistance(moves, labyrinth_copy.amountOfPossibleMoves, move, closestTile, allowsOpponentToReach(labyrinth_copy));
                freeLabyrinth(&labyrinth_copy);
            }
        }
    }

    if (winningMoves) {
        // Try to find the move that wins us but doesn't allow opponent to win
        for (int i = 0; i < winningMoves; i++) {
            if (moves[i].allowsOpponentToReach) continue;

            move = moves[i].move;
            move.x = moves[i].tile.x;
            move.y = moves[i].tile.y;
            return move;
        }
    } else {
        // Try to find the move (between the best 3) that gets us closest to item, us but doesn't allow opponent to win
        for (int i = 0; i < 3; i++) {
            if (!moves[i].initialized || moves[i].allowsOpponentToReach) continue;

            move = moves[i].move;
            move.x = moves[i].tile.x;
            move.y = moves[i].tile.y;
            return move;
        }
    }

    move = moves[0].move;
    move.x = moves[0].tile.x;
    move.y = moves[0].tile.y;
    return move;
}

/* Function: getWinner
 * Returns 1 if we win, -1 if opponent wins, 0 otherwise
 * Arguments:
 * - labyrinth: a labyrinth structure
 */
int getWinner (t_labyrinth labyrinth) {
    if ((labyrinth.me.starts && labyrinth.me.item-1 == labyrinth.me.finishingItem) ||
        (!labyrinth.me.starts && labyrinth.me.item+1 == labyrinth.me.finishingItem)){
        return 1;
    } else if(
        (labyrinth.opponent.starts && labyrinth.opponent.item-1 == labyrinth.opponent.finishingItem) ||
        (!labyrinth.opponent.starts && labyrinth.opponent.item+1 == labyrinth.opponent.finishingItem)) {
        return -1;
    }

    return 0;
}

/* Function: buildMinimaxGraph
 * Builds the minimax graph recursively and returns each time the score of the evaluated node
 * Arguments:
 * - labyrinth: a labyrinth structure
 * - maximizingPlayer: whether the current player is the maximizing or the minimizing player
 * - alpha: variable used for pruning (should be INT_MIN on initial call)
 * - beta: variable used for pruning (should be INT_MAX on initial call)
 * - depth: the current depth (should be 0 on initial call)
 * - maxDepth: the maximum depth to reach before evaluating the node score
 */
int buildMinimaxGraph (t_labyrinth labyrinth, t_node* node, int maximizingPlayer, int alpha, int beta, int depth, int maxDepth) {
    // Evaluate current position in the game if maxDepth is reached or the end is reached
    int endOfGameWinner = getWinner(labyrinth);
    if (depth == maxDepth || endOfGameWinner) {
        return (endOfGameWinner*24) + abs(labyrinth.opponent.item - labyrinth.opponent.finishingItem)-abs(labyrinth.me.item - labyrinth.me.finishingItem);
    }

    // Set initial score according to which player's turn it is
    if (maximizingPlayer) node->score = INT_MIN;
    else node->score = INT_MAX;

    // Allocate the necessary amount of space for potential children nodes
    node->children = calloc(labyrinth.amountOfPossibleMoves, sizeof(t_node));

    int correspondingSize[4] = {labyrinth.sizeY-1, labyrinth.sizeY-1, labyrinth.sizeX-1, labyrinth.sizeX-1};

    // Bruteforce all possible moves
    int nodeChildIndex = 0;
    t_move move;
    for (int insert = 0; insert < 4; insert++) {
        for (int number = 1; number < correspondingSize[insert]; number += 2) {
            if (labyrinth.forbiddenMove.insert == insert && labyrinth.forbiddenMove.number == number) continue;

            for (int rotation = 0; rotation < 4; rotation++) {
                // Create a deep copy of the labyrinth
                t_labyrinth labyrinth_copy;
                copyLabyrinth(labyrinth, &labyrinth_copy);

                // Select which player to simulate
                t_player* player;
                if (maximizingPlayer) player = &labyrinth_copy.me;
                else player = &labyrinth_copy.opponent;

                // Prepare the move and insert the tile
                move.insert = insert;
                move.number = number;
                move.rotation = rotation;
                insertExtraTile(&labyrinth_copy, move);

                // Update players positions and items
                movePlayer(labyrinth_copy, &labyrinth_copy.me, move);
                movePlayer(labyrinth_copy, &labyrinth_copy.opponent, move);

                // Find the player's item coordinates
                t_coordinates destination = getItemCoordinates(labyrinth_copy, player->item);

                // If item is ejected, skip the move
                if (destination.x == -1 && destination.y == -1) {
                    freeLabyrinth(&labyrinth_copy);
                    continue;
                }

                t_coordinates source = {.x = player->x, .y = player->y, .distance = abs(player->x - destination.x) + abs(player->y - destination.y)};

                // Attempt to reach destination or get the closest to it
                t_coordinates closestTile = source;
                if (isReachableOtherwiseClosest(labyrinth_copy, source, destination, source, &closestTile)) {
                    if (player->starts)
                        player->item++;
                    else
                        player->item--;

                    player->x = destination.x;
                    player->y = destination.y;
                } else {
                    player->x = closestTile.x;
                    player->y = closestTile.y;
                }

                // Update player coordinates
                move.x = player->x;
                move.y = player->y;

                // Recursively build the minimax graph
                node->children[nodeChildIndex].head = move;
                node->children[nodeChildIndex].initialized = 1;
                int score = buildMinimaxGraph(labyrinth_copy, &node->children[nodeChildIndex++], !maximizingPlayer, alpha, beta, depth + 1, maxDepth);

                // Maximize/Minimize the node's score
                if (maximizingPlayer) node->score = MAX(node->score, score);
                else node->score = MIN(node->score, score);

                // Update alpha/beta
                if (maximizingPlayer) alpha = MAX(alpha, score);
                else beta = MIN(beta, score);

                // Free the copied labyrinth
                freeLabyrinth(&labyrinth_copy);

                // Stop exploring if a better position already exists
                if (beta <= alpha)
                    return node->score;
            }
        }
    }

    return node->score;
}

/* Function: freeMinimaxGraph
 * Deep frees the given minimax graph
 * Arguments:
 * - labyrinth: a labyrinth structure
 * - node: the current node we're on (should be the head of the graph on the initial call)
 */
void freeMinimaxGraph(t_labyrinth labyrinth, t_node node) {
    // Deep search minimax graph
    int index = 0;
    while (index < labyrinth.amountOfPossibleMoves && node.children && node.children[index].initialized) {
        freeMinimaxGraph(labyrinth, node.children[index++]);
    }

    // Free node children if they exist
    if (node.children) free(node.children);
}

/* Function: minimax
 * Uses the minimax algorithm to return the best move evaluated
 * Arguments:
 * - labyrinth: a labyrinth structure
 * - move: the latest move
 * - myTurn: whether it is our turn or the opponent's turn
 * - maxDepth: the maximum depth to reach before evaluating the node score
 */
t_move minimax (t_labyrinth labyrinth, t_move move, int myTurn, int maxDepth) {
    // Build minimax graph
    t_node graph;
    graph.head = move;
    graph.score = 0;
    buildMinimaxGraph(labyrinth, &graph, myTurn, INT_MIN, INT_MAX, 0, maxDepth);

    // Find best move according to the graph
    int index = 0;
    int maxWeight = INT_MIN;
    int maxIndex;
    while (index < labyrinth.amountOfPossibleMoves && graph.children[index].initialized != 0) {
        if (graph.children[index].score > maxWeight) {
            maxWeight = graph.children[index].score;
            maxIndex = index;
        }
        index++;
    }

    // Free the graph and return the move
    t_move bestMove = graph.children[maxIndex].head;
    freeMinimaxGraph(labyrinth, graph);
    return bestMove;
}


/*
 * Strategie:
 *
 * Pour la fonction qui essaie de jouer le mieux posssible, on va essayer toutes les combinaisons d'insertions et de rotations
 * a chaque tour et pour chaque combinaison, on va retenir le Manhattan distance le plus petit entre le tresor, et le point
 * d'expansion courant. On retient les 2 meilleures insertions avec les distances de Manhattan les plus petites.
 * Si l'insertion fait sauter le tresor en dehors du labyrinthe, on ignore l'insertion.
 * On essaie d'effectuer la 1ere insertion. Si echec, on effectue la 2eme moins optimale.
 *
 * Bonus: essayer de semer le trouble a son adversaire
 *         Pour ce faire, si la partie qu'on a choisi de jouer qui nous rapproche le plus du tresor ne nous menes pas
 *         directement on tresor mais elle permet a l'adversaire d'arriver a son tresor avec une insertion,
 *         on essaie de trouver le coup qui ne laissera pas l'adversaire trouver le tresor, tout en respectant l'ordre
 *         d'importance (par distance Manhattan) des insertions deja calculees precedemment.
 *         Si malgre tout toutes les routes menent a Rome, on joue le tour le plus favorable pour nous.
 *
 * Bonus 2: construire le graphe du jeu et essayer de mener le jeu de facon a ce que l'on gagne le plus de fois possibles
 *          Usage de l'algorithme minimax.
 *          Formule empirique:
 *          P = abs(tresorAdversaire - tresorFinalAdversaire)-abs(monTresor - monTresorFinal) + (24 si je gagne/-24 si adversaire gagne/0 sinon)
 *          Plus P est grand, plus on a des chances de gagner
 *
 * Benchmarks:
 * - Manhattan Distance: 75% win rate versus BASIC
 * - Manhattan Distance + Anti-win algorithm: 75% win rate versus BASIC
 * - Euclidian Distance: 74% win rate versus BASIC
 * - Euclidian Distance + Anti-win algorithm: 74% win rate versus BASIC
 * - Minimax: Very variable win rate (50-90%)
 */