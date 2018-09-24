#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include <ncurses.h>

#include "config.h"
#include "shuffle.h"

// Define constant directions
#define N 1
#define S 2
#define E 4
#define W 8

int directions_array[4] = {N, S, E, W};
int directions[4] = {N, S, E, W};
int DX (int direction) {
  int retval = 0;
  switch (direction) {
  case E:
    retval = 1;
    break;
  case W:
    retval = -1;
    break;
  default:
    retval = 0;
  }
  return retval;
}

int DY (int direction) {
  if (direction == N) {
    return -1;
  } else if (direction == S) {
    return 1;
  } else return 0;
}

int OPPOSITE (int direction) {
  switch (direction) {
  case N:
    return S;
  case S:
    return N;
  case E:
    return W;
  case W:
    return E;
  default:
    return 0;
  }
}

// Recursive Backtracking Algorithm
int ** carve_passages(int width, int height, int cx, int cy, int ** maze) {
  if (maze == NULL) {
    maze = calloc(height, sizeof(int*));
    if (maze == NULL) {
      fprintf(stderr, "Failure creating maze int **\n");
      return NULL;
    }
    for (int i = 0; i < height; i++) {
      maze[i] = calloc(width, sizeof(int));
      if (maze[i] == NULL) {
        fprintf(stderr, "Failure creating maze sub-arrays\n");
        return NULL;
      }
    }
  }
  // Each int is a OR'd list of directions where there are NOT walls (ie, where there are passages)
  shuffle_int(directions, 4);
  for (int i = 0; i < 4; i++) {
    int direction = directions[i];
    int nx = cx + DX(direction);
    int ny = cy + DY(direction);
    if ((ny <= height - 1 && ny >= 0) && (nx <= width - 1 && nx >= 0) && maze[ny][nx] == 0) {
      maze[cy][cx] |= direction;
      maze[ny][nx] |= OPPOSITE(direction);
      maze = carve_passages(width, height, nx, ny, maze);
    }
  }
  if (maze == NULL) {
    fprintf(stderr, "there was an error??\n");
  }
  return maze;
}

char * genmaze (int width, int height, char wallchar, char floorchar, int ** maze) {
  if (width < 2) {
    return NULL;
  }
  if (height < 2) {
    return NULL;
  }
  if (!isprint(wallchar)) {
    wallchar = '#';
  }
  if (!isprint(floorchar)) {
    floorchar = '.';
  }
  int maze_given = 1;
  char * ret = NULL;
  if (maze == NULL) {
    maze_given = 0;
    maze = carve_passages((width-1)*2, (height-1)*2, 0, 0, maze);
  }
  if (maze == NULL) {
    fprintf(stderr, "Error creating maze.\n");
    return NULL;
  }
  ret = malloc(width*height+1);
  if (ret == NULL) {
    fprintf(stderr, "Error allocating char-array");
    return NULL;
  }
  memset(ret, wallchar, width*height);
  ret[width*height] = 0;
  ret[width] = floorchar; // Entrance to maze is open
  for (int y = 0; y < (height - 1) / 2; y++) {
    for (int x = 0; x < (width - 1) / 2; x++) {
      ret[(y*width*2)+(x*2)] = floorchar;
      for (int d = 0; d < 4; d++) { // 4 directions, loop through each
        int direction = 1 << d;
        if ((maze[y][x] & direction) == direction) {
          ret[(y+ DY(direction) * width * 2 + 1)+(x*2+1) + DX(direction)] = floorchar; // Possibly destroy wall
        }
      } // Direction loop
    } // x loop
  } // y loop
  ret[(height*2*width+1)+(width*2+1)] = floorchar;
  if (maze_given == 0) {
    for (int i = 0; i < (height-1)*2; i++) {
      free(maze[i]);
    }
    free(maze);
  }
  return ret;
}
