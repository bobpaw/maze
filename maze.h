#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "config.h"
#include "shuffle.h"

// Define constant directions
#define N 1
#define S 2
#define E 4
#define W 8

#ifndef MAZE_MAZE_H_
#define MAZE_MAZE_H_

int ** carve_passages(int width, int height, int cx, int cy, int ** maze);
char * genmaze (int width, int height, int * retwidth, int * retheight, char wallchar, char floorchar, int ** maze);
int correct_distance (const int width, const int height, int ** maze, const char * travel_string);

#endif // MAZE_MAZE_H_

