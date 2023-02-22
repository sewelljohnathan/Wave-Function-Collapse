
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WORLD_W 50
#define WORLD_H 25
#define TILES 4

typedef struct Potential {
    int options[TILES];
} Potential;

typedef struct World {
    Potential map[WORLD_H][WORLD_W];
} World;


void collapseLocal(int collapseTarget, int offsetX, int offsetY, World *world);
void collapse(World *world);
char* potentialMapping(Potential potential);

int main() {

    // Seed random number gen
    srand(time(NULL));

    // Create the world
    World world;
    int isValidWorld;
    do {

        // Initialize all grid potentials to 1
        for (int y = 0; y < WORLD_H; y++) {
            for (int x = 0; x < WORLD_W; x++) {
                for (int i = 0; i < TILES; i++) {
                    world.map[y][x].options[i] = 1;
                }
            }
        }

        // Collapse
        collapse(&world);

        // Check validity
        isValidWorld = 1;
        for (int y = 0; y < WORLD_H; y++) {
            for (int x = 0; x < WORLD_W; x++) {
                
                // Entropy should be 1 for all cells if collapsed correctly
                int entropy = 0;
                for (int i = 0; i < TILES; i++) {
                    entropy += world.map[y][x].options[i];
                }

                if (entropy == 0) {
                    isValidWorld = 0;
                }
            }
        }

    } while (!isValidWorld);

    // Display
    for (int y = 0; y < WORLD_H; y++) {
        char worldRow[WORLD_W];
        for (int x = 0; x < WORLD_W; x++) {
            printf("%s ", potentialMapping(world.map[y][x]));
            //worldRow[x] = potentialMapping(world.map[y][x]);
        }
        printf("\e[0m\n");
    }

    return 0;
}

char* potentialMapping(Potential potential) {
    if (potential.options[0]) {
        return "\e[104m";
    }
    if (potential.options[1]) {
        return "\e[43m";
    }
    if (potential.options[2]) {
        return "\e[102m";
    }
    if (potential.options[3]) {
        return "\e[42m";
    }
}

void collapseLocal(int collapseTarget, int y, int x, World *world) {
    switch (collapseTarget) {
        case 0:
            world->map[y][x].options[2] = 0;
            world->map[y][x].options[3] = 0;
        break;
        case 1:
            world->map[y][x].options[3] = 0;
        break;
        case 2:
            world->map[y][x].options[0] = 0;
        break;
        case 3:
            world->map[y][x].options[0] = 0;
            world->map[y][x].options[1] = 0;
        break;
    }
}

void collapse(World *world) {

    // Find the lowest entropy cell
    int lowestEntropy = TILES+1;
    int lowestY = 0;
    int lowestX = 0;
    int foundCell = 0;
    for (int y = 0; y < WORLD_H; y++) {
        for (int x = 0; x < WORLD_W; x++) {

            int entropy = 0;
            for (int i = 0; i < TILES; i++) {
                entropy += world->map[y][x].options[i];   
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
    } while (world->map[lowestY][lowestX].options[randCollapse] == 0);
    
    // Collapse
    for (int i = 0; i < TILES; i++) {
        if (i == randCollapse) {
            continue;
        }
        world->map[lowestY][lowestX].options[i] = 0;
        
    }

    // Collapse surroundings
    if (lowestY > 0) {
        collapseLocal(randCollapse, lowestY-1, lowestX, world);
    }
    if (lowestY < WORLD_H - 1) {
        collapseLocal(randCollapse, lowestY+1, lowestX, world);
    }
    if (lowestX > 0) {
        collapseLocal(randCollapse, lowestY, lowestX-1, world);
    }
    if (lowestX < WORLD_W - 1) {
        collapseLocal(randCollapse, lowestY, lowestX+1, world);
    }

    collapse(world);
}
