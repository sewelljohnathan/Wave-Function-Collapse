
#define WORLD_W 10
#define WORLD_H 10
#define TILE_COUNT 7

typedef struct Cell {
    int options[TILE_COUNT];
    int collapsedValue;
} Cell;

typedef struct World {
    Cell map[WORLD_H][WORLD_W];
} World;

typedef int Tile[3][3];

const Tile tiles[TILE_COUNT] = {
    {
        {0,0,0},
        {0,0,0},
        {0,0,0},
    },
    {
        {0,1,0},
        {1,1,1},
        {0,0,0},
    },
    {
        {0,0,0},
        {1,1,1},
        {0,1,0},
    },
    {
        {0,1,0},
        {1,1,0},
        {0,1,0},
    },
    {
        {0,1,0},
        {0,1,1},
        {0,1,0},
    },
    {
        {0,1,0},
        {0,1,0},
        {0,1,0},
    },
    {
        {0,0,0},
        {1,1,1},
        {0,0,0},
    },
};
