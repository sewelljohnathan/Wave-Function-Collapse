
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"

void collapseLocal(int collapseTarget, int offsetX, int offsetY, World *world);
void collapse(World *world);
char* getColor(int collapseValue);

int main() {

    // Seed random number gen
    srand(time(NULL));

    // Create the world
    World world;
    int isValidWorld;

    // Initialize all grid potentials to 1
    for (int y = 0; y < WORLD_H; y++) {
        for (int x = 0; x < WORLD_W; x++) {
            for (int i = 0; i < TILE_COUNT; i++) {
                world.map[y][x].options[i] = 1;
                world.map[y][x].collapsedValue = -1;
            }
        }
    }

    // Collapse
    collapse(&world);

    // Display
    for (int y = 0; y < WORLD_H; y++) {
        for (int i = 0; i < 3; i++) {
            for (int x = 0; x < WORLD_W; x++) {
                for (int j = 0; j < 3; j++) {
                    printf("%s  ", getColor(tiles[world.map[y][x].collapsedValue][i][j]));
                }
            }
            printf("\e[0m\n");
        }
    }

    return 0;
}

char* getColor(int val) {
    switch (val) {
        case 0: return "\e[106m"; break;
        case 1: return "\e[41m"; break;
    }
}

void collapseLocal(int collapseTarget, int y, int x, World *world) {

    for (int i = 0; i < TILE_COUNT; i++) {

        if (y-1 >= 0) {
            if (
                tiles[i][2][0] != tiles[collapseTarget][0][0] ||
                tiles[i][2][1] != tiles[collapseTarget][0][1] ||
                tiles[i][2][2] != tiles[collapseTarget][0][2]
            ) {
                world->map[y-1][x].options[i] = 0;
            }
        }
        if (y+1 <= WORLD_H - 1) {
            if (
                tiles[i][0][0] != tiles[collapseTarget][2][0] ||
                tiles[i][0][1] != tiles[collapseTarget][2][1] ||
                tiles[i][0][2] != tiles[collapseTarget][2][2]
            ) {
                world->map[y+1][x].options[i] = 0;
            }
        }
        if (x-1 >= 0) {
            if (
                tiles[i][0][2] != tiles[collapseTarget][0][0] ||
                tiles[i][1][2] != tiles[collapseTarget][1][0] ||
                tiles[i][2][2] != tiles[collapseTarget][2][0]
            ) {
                world->map[y][x-1].options[i] = 0;
            }
        }
        if (x+1 <= WORLD_W - 1) {
            if (
                tiles[i][0][0] != tiles[collapseTarget][0][2] ||
                tiles[i][1][0] != tiles[collapseTarget][1][2] ||
                tiles[i][2][0] != tiles[collapseTarget][2][2]
            ) {
                world->map[y][x+1].options[i] = 0;
            }
        }
    }
}

void collapse(World *world) {

    // Find the lowest entropy cell
    int lowestEntropy = TILE_COUNT+1;
    int lowestY = 0;
    int lowestX = 0;
    int foundCell = 0;
    for (int y = 0; y < WORLD_H; y++) {
        for (int x = 0; x < WORLD_W; x++) {

            int entropy = 0;
            for (int i = 0; i < TILE_COUNT; i++) {
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
        collapseValue = rand() % TILE_COUNT;
    } while (world->map[lowestY][lowestX].options[collapseValue] == 0);

    // Collapse
    for (int i = 0; i < TILE_COUNT; i++) {
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
