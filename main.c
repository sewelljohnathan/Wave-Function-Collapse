
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WORLD_SIZE 20
#define TILES 3

typedef struct Potential {
    int potential[TILES];
} Potential;

typedef struct World {
    Potential map[WORLD_SIZE][WORLD_SIZE];
} World;


void collapseLocal(int collapseTarget, int offsetX, int offsetY, World *world);
void collapse(World *world);
char potentialMapping(Potential potential);

int main() {

    // Seed random number gen
    srand(time(NULL));

    // Create the world
    World world;

    // Initialize all grid potentials to 1
    for (int y = 0; y < WORLD_SIZE; y++) {
        for (int x = 0; x < WORLD_SIZE; x++) {
            for (int i = 0; i < TILES; i++) {
                world.map[y][x].potential[i] = 1;
            }
        }
    }

    // Collapse
    collapse(&world);

    // Display
    for (int y = 0; y < WORLD_SIZE; y++) {
        char worldRow[WORLD_SIZE];
        for (int x = 0; x < WORLD_SIZE; x++) {
            worldRow[x] = potentialMapping(world.map[y][x]);
        }
        printf("%s\n", worldRow);
    }
    

    return 0;
}

char potentialMapping(Potential potential) {
    if (potential.potential[0]) {
        return ' ';
    }
    if (potential.potential[1]) {
        return '-';
    }
    if (potential.potential[2]) {
        return '#';
    }
}

void collapseLocal(int collapseTarget, int y, int x, World *world) {
    switch (collapseTarget) {
        case 0:
            world->map[y][x].potential[2] = 0;
        break;
        case 2:
            world->map[y][x].potential[0] = 0;
        break;
    }
}

void collapse(World *world) {

    // Find the lowest entropy cell
    int lowestEntropy = TILES+1;
    int lowestY = 0;
    int lowestX = 0;
    int foundCell = 0;
    for (int y = 0; y < WORLD_SIZE; y++) {
        for (int x = 0; x < WORLD_SIZE; x++) {

            int entropy = 0;
            for (int i = 0; i < TILES; i++) {
                entropy += world->map[y][x].potential[i];   
            }

            if (entropy < lowestEntropy && entropy > 1) {
                lowestY = y;
                lowestX = x;
                lowestEntropy = entropy;

                foundCell = 1;
            }
        }
    }

    // All cells have been collapsed
    if (!foundCell) {
        return;
    }

    // Cannot collapse to something that is already decided to be impossible (0)
    int randCollapse;
    do {
        int r = (double) rand() / (double) RAND_MAX;
        randCollapse = rand() % TILES;
    } while (world->map[lowestY][lowestX].potential[randCollapse] == 0);
    
    // Collapse
    for (int i = 0; i < TILES; i++) {
        if (i == randCollapse) {
            continue;
        }
        world->map[lowestY][lowestX].potential[i] = 0;
        
    }

    // Collapse surroundings
    if (lowestY > 0) {
        collapseLocal(randCollapse, lowestY-1, lowestX, world);
    }
    if (lowestY < WORLD_SIZE - 1) {
        collapseLocal(randCollapse, lowestY+1, lowestX, world);
    }
    if (lowestX > 0) {
        collapseLocal(randCollapse, lowestY, lowestX-1, world);
    }
    if (lowestX < WORLD_SIZE - 1) {
        collapseLocal(randCollapse, lowestY, lowestX+1, world);
    }

    collapse(world);
}