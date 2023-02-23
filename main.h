
#define WORLD_W 20
#define WORLD_H 20
#define TILES 5

typedef struct Potential {
    int options[TILES];
    int collapsedValue;
} Potential;

typedef struct World {
    Potential map[WORLD_H][WORLD_W];
} World;

typedef char Tile[3][3][7];
typedef enum TileType {
    Blank = 0, Up, Down, Left, Right
} TileType;


Tile BlankTile = {
    {"\e[106m", "\e[106m","\e[106m"},
    {"\e[106m", "\e[106m","\e[106m"},
    {"\e[106m", "\e[106m","\e[106m"}
};
Tile UpTile = {
    {"\e[106m", "\e[41m","\e[106m"},
    {"\e[41m", "\e[41m","\e[41m"},
    {"\e[106m", "\e[106m","\e[106m"}
};
Tile DownTile = {
    {"\e[106m", "\e[106m","\e[106m"},
    {"\e[41m", "\e[41m","\e[41m"},
    {"\e[106m", "\e[41m","\e[106m"}
};
Tile LeftTile = {
    {"\e[106m", "\e[41m","\e[106m"},
    {"\e[41m", "\e[41m","\e[106m"},
    {"\e[106m", "\e[41m","\e[106m"}
};
Tile RightTile = {
    {"\e[106m", "\e[41m","\e[106m"},
    {"\e[106m", "\e[41m","\e[41m"},
    {"\e[106m", "\e[41m","\e[106m"}
};
