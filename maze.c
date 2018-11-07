/*
    A simple maze generator
    Copyright (C) 2018 Aiden Woodruff

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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

static int directions_array[4] = {N, S, E, W};
static int directions[4] = {N, S, E, W};
static int DX (int direction) {
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

static int DY (int direction) {
  if (direction == N) {
    return -1;
  } else if (direction == S) {
    return 1;
  } else return 0;
}

static int OPPOSITE (int direction) {
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

char * genmaze (int width, int height, int * retwidth, int * retheight, char wallchar, char floorchar, int ** maze) {
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
  int retsize = (2*height+1)*(2*width+1)+1;
  if (maze == NULL) {
    maze_given = 0;
    maze = carve_passages(width, height, 0, 0, maze);
  }
  if (maze == NULL) {
    fprintf(stderr, "Error creating maze.\n");
    return NULL;
  }
  ret = malloc(retsize);
  if (ret == NULL) {
    fprintf(stderr, "Error allocating char-array");
    return NULL;
  }
  memset(ret, wallchar, retsize-1);
  ret[retsize-1] = 0;
  ret[2*width+1] = floorchar; // Entrance to maze is open
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      ret[(2*width+1)*(2*y+1)+2*x+1] = floorchar;
      for (int d = 0; d < 4; d++) { // 4 directions, loop through each
        int direction = 1 << d;
        if ((maze[y][x] & direction) == direction) {
          ret[(2*width+1)*(2*y+1+DY(direction))+2*x+1 + DX(direction)] = floorchar; // Possibly destroy wall
        }
      } // Direction loop
    } // x loop
  } // y loop
  ret[(retsize - 2) - (2*width+1)] = floorchar;
  if (maze_given == 0) {
    for (int i = 0; i < height; i++) {
      free(maze[i]);
    }
    free(maze);
  }
  *retwidth = 2*width+1;
  *retheight = 2*height+1;
  return ret;
}
