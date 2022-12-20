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

void printRawLabyrinthDebug(int* labyrinth, int sizeX, int sizeY);

void printLabyrinthDebug(t_labyrinth labyrinth);

void initLabyrinth(t_labyrinth* labyrinth, int* temp_labyrinth, int myTurn);

#endif
