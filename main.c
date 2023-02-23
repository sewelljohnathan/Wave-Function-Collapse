
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"

void collapseLocal(int collapseTarget, int offsetX, int offsetY, World *world);
void collapse(World *world);
Tile* getTile(Potential potential);

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
        for (int i = 0; i < 3; i++) {
            for (int x = 0; x < WORLD_W; x++) {
                Tile *tile = getTile(world.map[y][x]);
                for (int j = 0; j < 3; j++) {
                    printf("%s  ", (*tile)[i][j]);
                }
            }
            printf("\e[0m\n");
        }
    }

    return 0;
}

Tile* getTile(Potential potential) {

    switch(potential.collapsedValue) {
        case 0: return &BlankTile; break;
        case 1: return &UpTile; break;
        case 2: return &DownTile; break;
        case 3: return &LeftTile; break;
        case 4: return &RightTile; break;
    }
    exit(1);
}

void collapseLocal(int collapseTarget, int y, int x, World *world) {

    switch (collapseTarget) {
        case Blank:
            if (y-1 >= 0) {
                world->map[y-1][x].options[Down] = 0;
                world->map[y-1][x].options[Left] = 0;
                world->map[y-1][x].options[Right] = 0;
            }
            if (y+1 <= WORLD_H - 1) {
                world->map[y+1][x].options[Up] = 0;
                world->map[y+1][x].options[Left] = 0;
                world->map[y+1][x].options[Right] = 0;
            }
            if (x-1 >= 0) {
                world->map[y][x-1].options[Up] = 0;
                world->map[y][x-1].options[Down] = 0;
                world->map[y][x-1].options[Right] = 0;
            }
            if (x+1 <= WORLD_W - 1) {
                world->map[y][x+1].options[Up] = 0;
                world->map[y][x+1].options[Down] = 0;
                world->map[y][x+1].options[Left] = 0;
            }
        break;
        case Up:
            if (y-1 >= 0) {
                world->map[y-1][x].options[Blank] = 0;
                world->map[y-1][x].options[Up] = 0;
            }
            if (y+1 <= WORLD_H - 1) {
                world->map[y+1][x].options[Up] = 0;
                world->map[y+1][x].options[Left] = 0;
                world->map[y+1][x].options[Right] = 0;
            }
            if (x-1 >= 0) {
                world->map[y][x-1].options[Blank] = 0;
                world->map[y][x-1].options[Left] = 0;
            }
            if (x+1 <= WORLD_W - 1) {
                world->map[y][x+1].options[Blank] = 0;
                world->map[y][x+1].options[Right] = 0;
            }
        break;
        case Down:
            if (y-1 >= 0) {
                world->map[y-1][x].options[Down] = 0;
                world->map[y-1][x].options[Left] = 0;
                world->map[y-1][x].options[Right] = 0;
            }
            if (y+1 <= WORLD_H - 1) {
                world->map[y+1][x].options[Blank] = 0;
                world->map[y+1][x].options[Down] = 0;
            }
            if (x-1 >= 0) {
                world->map[y][x-1].options[Blank] = 0;
                world->map[y][x-1].options[Left] = 0;
            }
            if (x+1 <= WORLD_W - 1) {
                world->map[y][x+1].options[Blank] = 0;
                world->map[y][x+1].options[Right] = 0;
            }
        break;
        case Left:
            if (y-1 >= 0) {
                world->map[y-1][x].options[Blank] = 0;
                world->map[y-1][x].options[Up] = 0;
            }
            if (y+1 <= WORLD_H - 1) {
                world->map[y+1][x].options[Blank] = 0;
                world->map[y+1][x].options[Down] = 0;
            }
            if (x-1 >= 0) {
                world->map[y][x-1].options[Blank] = 0;
                world->map[y][x-1].options[Left] = 0;
            }
            if (x+1 <= WORLD_W - 1) {
                world->map[y][x+1].options[Up] = 0;
                world->map[y][x+1].options[Down] = 0;
                world->map[y][x+1].options[Left] = 0;
            }
        break;
        case Right:
            if (y-1 >= 0) {
                world->map[y-1][x].options[Blank] = 0;
                world->map[y-1][x].options[Up] = 0;
            }
            if (y+1 <= WORLD_W - 1) {
                world->map[y+1][x].options[Blank] = 0;
                world->map[y+1][x].options[Down] = 0;
            }
            if (x-1 >= 0) {
                world->map[y][x-1].options[Up] = 0;
                world->map[y][x-1].options[Down] = 0;
                world->map[y][x-1].options[Right] = 0;
            }
            if (x+1 <= WORLD_W - 1) {
                world->map[y][x+1].options[Blank] = 0;
                world->map[y][x+1].options[Right] = 0;
            }
        break;
        default:
            printf("Error!");
            exit(1);
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

            if (entropy < lowestEntropy && world->map[y][x].collapsedValue == -1) {
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

    // Get collapsed value
    int collapseValue;
    do {
        collapseValue = rand() % TILES;
    } while (world->map[lowestY][lowestX].options[collapseValue] == 0);

    // Collapse
    for (int i = 0; i < TILES; i++) {
        if (i != collapseValue) {
            world->map[lowestY][lowestX].options[i] = 0;
        }
    }
    world->map[lowestY][lowestX].collapsedValue = collapseValue;

    // Propagate to neighbors
    collapseLocal(collapseValue, lowestY, lowestX, world);

    // Collapse new world
    collapse(world);
}
