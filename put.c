#include <stdlib.h>
#include <stdio.h>

#include "maze.h"

int main (int argc, char * argv[]) {
  int width = 16;
  int height = 16;
  if (argc > 2) {
    width = atoi(argv[1]);
    height = atoi(argv[2]);
  }
  int ** maze = NULL;
  maze = calloc(height, sizeof(int*));
  if (maze == NULL) {
    fprintf(stderr, "Failure creating maze int **\n");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < height; i++) {
    maze[i] = calloc(width, sizeof(int));
    if (maze[i] == NULL) {
      fprintf(stderr, "Failure creating maze sub-arrays\n");
      exit(EXIT_FAILURE);
    }
  }
  carve_passages(width, height, 0, 0, maze);
  printf(" ");
  for (int i = 0; i < (width * 2 - 1); i++) {
    printf("_");
  }
  printf("\n");
  for (int y = 0; y < height; y++) {
    printf("|");
    for (int x = 0; x < width; x++) {
      printf(((maze[y][x] & S) != 0) ? " " : "_");
      if ((maze[y][x] & E) != 0) {
        printf((((maze[y][x] | maze[y][x+1]) & S) != 0) ? " " : "_");
      } else {
        printf("|");
      }
    }
    printf("\n");
  }
  return 0;
}
