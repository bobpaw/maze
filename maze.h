#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <ncurses.h>

#define N 1
#define S 2
#define E 4
#define W 8

int directions_array[4] = {N, S, E, W};
int DX (int direction) {
  if (direction == E) {
    return 1;
  } else if (direction == W) {
    return -1;
  } else return 0;
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
  srand(time(NULL));
  if (maze == NULL) {
    maze = calloc(height, sizeof(int*));
    if (maze == NULL) {
      perror("Failure creating array of pointers");
      return NULL;
    }
    for (int i = 0; i < height; i++) {
      maze[i] = calloc(width, sizeof(int));
      if (maze[i] == NULL) {
        perror("Failure creating sub-arrays");
        return NULL;
      }
    }
  }
  int directions[4] = {0,0,0,0};
  {
    int directions_used = 0;
    int cur_d = 0;
    for (int i = 0; i < 3; i++) {
      while (directions_used & (1 << cur_d) != 0) cur_d = (rand() % 4);
      directions_used |= (1 << cur_d);
      directions[i] = (1 << cur_d);
    }
  }
  for (int i = 0; i < 4; i++) {
    int direction = 1 << i;
    int nx = cx + DX(direction);
    int ny = cy + DY(direction);
    if ((ny < height - 1 && ny > 0) && (nx < width - 1 && nx > 0) && maze[ny][nx] == 0) {
      maze[cy][cx] |= direction;
      maze[ny][nx] |= OPPOSITE(direction);
      carve_passages(width, height, nx, ny, maze);
    }
  }
  return 0;
}

char * genmaze (int width, int height, char wallchar, char floorchar) {
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
  char * ret = NULL;
  int ** maze = NULL;
  carve_passages((width-1)*2, (height-1)*2, 0, 0, maze);
  if (maze == NULL) {
    return NULL;
  }
  ret = malloc(width*height+1);
  if (ret == NULL) {
    perror("Error allocating char-array");
    return NULL;
  }
  /* Actually Translate the maze
     La
     Di
     Da
  */
  for (int i = 0; i < (height-1)*2; i++) {
    free(maze[i]);
  }
  free(maze);
  return ret;
}
