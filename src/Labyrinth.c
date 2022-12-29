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
    // set opponent's initial position according to who starts
    labyrinth->opponent.x = (myTurn) ? labyrinth->sizeX-1 : 0;
    labyrinth->opponent.y = (myTurn) ? labyrinth->sizeY-1 : 0;

    // set forbidden move to not existant
    labyrinth->forbiddenMove.insert = -1;
    labyrinth->forbiddenMove.number = 0;

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
        tile->North = tile->West;
        tile->East = tile->North;
        tile->South = tile->East;
        tile->West = tile->South;
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
    // move lines/columns according to the inserted tile
    if (move.insert == INSERT_LINE_LEFT) {
        for (int i = labyrinth->sizeX-2; i >= 0; i--) {
            labyrinth->tiles[move.number][i+1] = labyrinth->tiles[move.number][i];
        }
        labyrinth->tiles[move.number][0] = labyrinth->extraTile;

    } else if (move.insert == INSERT_LINE_RIGHT) {
        for (int i = 1; i < labyrinth->sizeX; i++) {
            labyrinth->tiles[move.number][i-1] = labyrinth->tiles[move.number][i];
        }
        labyrinth->tiles[move.number][labyrinth->sizeX - 1] = labyrinth->extraTile;
    } else if (move.insert == INSERT_COLUMN_TOP) {
        for (int i = labyrinth->sizeY-2; i >= 0; i--) {
            labyrinth->tiles[i+1][move.number] = labyrinth->tiles[i][move.number];
        }
        labyrinth->tiles[0][move.number] = labyrinth->extraTile;
    } else if (move.insert == INSERT_COLUMN_BOTTOM) {
        for (int i = 1; i < labyrinth->sizeY; i++) {
            labyrinth->tiles[i-1][move.number] = labyrinth->tiles[i][move.number];
        }
        labyrinth->tiles[labyrinth->sizeY-1][move.number] = labyrinth->extraTile;
    }
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
}

/* Function: updateLabyrinth
 * Updates the labyrinth structure when the opponent makes a move
 * Arguments:
 * - labyrinth: a pointer to the labyrinth structure
 * - move: the move which was done
 */
void updateLabyrinth(t_labyrinth* labyrinth, t_move move) {
    // Update the forbidden move
    int oppositeMoves[4] = {1, 0, 3, 2};
    labyrinth->forbiddenMove.insert = oppositeMoves[move.insert];

    labyrinth->forbiddenMove.number = move.number;

    rotateTile(&labyrinth->extraTile, move.rotation);
    insertExtraTile(labyrinth, move);

    // Update the extra tile
    labyrinth->extraTile.North = move.tileN;
    labyrinth->extraTile.East = move.tileE;
    labyrinth->extraTile.South = move.tileS;
    labyrinth->extraTile.West = move.tileW;
    labyrinth->extraTile.Item = move.tileItem;

    // Update players positions and items
    movePlayer(*labyrinth, &labyrinth->me, move);
    labyrinth->opponent.x = move.x;
    labyrinth->opponent.y = move.y;
    labyrinth->opponent.item = move.nextItem;
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

/*
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
 */