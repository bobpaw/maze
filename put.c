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

#include "maze.h"

int main (int argc, char * argv[]) {
  srandom(clock());
  int width = 10;
  int height = 10;
  int retwidth = 0;
  int retheight = 0;
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
  printf("done\n");
  if ((argc == 4 && strcmp(argv[3], "-c") == 0) || (argc == 2 && strcmp(argv[1], "-c") == 0)) {
    char * map = NULL;
    map = genmaze(width, height, &retwidth, &retheight, '#', '.', maze);
    for (int i = 0; i < retheight; i++) {
    printf("%.*s\n", retwidth, map+i*retwidth);
    }
  }
  return 0;
}
