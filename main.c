
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WORLD_W 50
#define WORLD_H 25
#define TILES 4

typedef struct Potential {
    int options[TILES];
    int collapsedValue;
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
                    world.map[y][x].collapsedValue = -1;
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
        for (int x = 0; x < WORLD_W; x++) {
            printf("%s ", potentialMapping(world.map[y][x]));
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

int getCollapseValue(World *world, int y, int x) {
    
    // Cannot collapse to something that is already decided to be impossible (0)
    int randCollapse;
    do {
        
        // Give all options an equal possibility
        int weights[TILES];
        for (int i = 0; i < TILES; i++) {
            weights[i] = 3;
        }

        // Add a bias for a neighboring collapsed value
        int dyArr[] = {-1, -1, -1,  0,  0,  1,  1,  1};
        int dxArr[] = {-1,  0,  1, -1,  1, -1,  0,  1};
        for (int i = 0; i < 8; i++) {
            int dy = dyArr[i];
            int dx = dxArr[i];
            if (y+dy >= 0 && y+dy <= WORLD_H - 1 && x+dx >= 0 && x+dx <= WORLD_W - 1) {
                int neighborValue = world->map[y+dy][x+dx].collapsedValue;
                if (neighborValue != -1) {
                    weights[neighborValue] += 1;
                }
            }
        }

        // Get the total of all weights
        int totalWeight = 0;
        for (int i = 0; i < TILES; i++) {
            totalWeight += weights[i];   
        }

        // Get an ascending summed list of weights
        // I.e. If the weights are {2, 1, 2, 3}, preppedWeights is {2, 3, 5, 8}
        int preppedWeights[TILES];
        for (int i = 0; i < TILES; i++) {
            preppedWeights[i] = 0;
            for (int j = 0; j <= i; j++) {
                preppedWeights[i] += weights[j];
            }
        }

        int r = rand() % totalWeight;
        for (int i = 0; i < TILES; i++) {
            if (r <= preppedWeights[i]) {
                randCollapse = i;
                break;
            }
        }

        //randCollapse = rand() % TILES;
    } while (world->map[y][x].options[randCollapse] == 0);

    return randCollapse;
    
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

    // Collapse
    int collapseValue = getCollapseValue(world, lowestY, lowestX);
    for (int i = 0; i < TILES; i++) {
        if (i == collapseValue) {
            continue;
        }
        world->map[lowestY][lowestX].options[i] = 0;
    }
    world->map[lowestY][lowestX].collapsedValue = collapseValue;

    // Propagate to neighbors
    int dyArr[] = {-1, -1, -1,  0,  0,  1,  1,  1};
    int dxArr[] = {-1,  0,  1, -1,  1, -1,  0,  1};
    for (int i = 0; i < 8; i++) {
        int localY = lowestY + dyArr[i];
        int localX = lowestX + dxArr[i];
        if (localY >= 0 && localY <= WORLD_H - 1 && localX >= 0 && localX <= WORLD_W - 1) {
            collapseLocal(collapseValue, localY, localX, world);
        }
    }

    collapse(world);
}
