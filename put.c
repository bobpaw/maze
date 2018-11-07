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
#include "cmdline-put.h"

int main (int argc, char * argv[]) {
  srandom(clock());
  int width = 10;
  int height = 10;
  int retwidth = 0;
  int retheight = 0;
  struct gengetopt_args_info args_info;
  int ** maze = NULL;
  if (cmdline_parser(argc, argv, &args_info) != 0) {
    fprintf(stderr, "Error processing command line arguments\n");
    exit(EXIT_FAILURE);
  }
  if (args_info.width_given) {
    width = args_info.width_arg;
  } else {
    width = 10;
  }
  if (args_info.height_given) {
    height = args_info.height_arg;
  } else {
    height = 10;
  }
  maze = calloc(height, sizeof(int*));
  if (maze == NULL) {
    fprintf(stderr, "Failure creating maze int **\n");
    cmdline_parser_free(&args_info);
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < height; i++) {
    maze[i] = calloc(width, sizeof(int));
    if (maze[i] == NULL) {
      fprintf(stderr, "Failure creating maze sub-arrays\n");
      cmdline_parser_free(&args_info);
      exit(EXIT_FAILURE);
    }
  }
  carve_passages(width, height, 0, 0, maze);
  if (args_info.walls_given || args_info.both_given || !args_info.cells_given) {
    putchar(' ');
    for (int i = 0; i < (width * 2 - 1); i++) {
      putchar('_');
    }
    putchar('\n');
    for (int y = 0; y < height; y++) {
      putchar('|');
      for (int x = 0; x < width; x++) {
        putchar(((maze[y][x] & S) != 0) ? ' ' : '_');
        if ((maze[y][x] & E) != 0) {
          putchar((((maze[y][x] | maze[y][x+1]) & S) != 0) ? ' ' : '_');
        } else {
          putchar('|');
        }
      }
      putchar('\n');
    }
  }
  if (args_info.cells_given || args_info.both_given) {
    char * map = NULL;
    map = genmaze(width, height, &retwidth, &retheight, '#', '.', maze);
    for (int i = 0; i < retheight; i++) {
      printf("%.*s\n", retwidth, map+i*retwidth);
    }
    free(map);
  }
  if (args_info.guess_given) {
    char * far = malloc(height * width);
    memset(far, 0, height * width);
    size_t far_s = height * width;
    printf("Just q to exit.\n");
    while (far[0] != 'q') {
      printf("Proposed Solution: ");
      getline(&far, &far_s, stdin);
      printf("%d\n", correct_distance(width, height, maze, (const char *) far));
    }
    free(far);
  }
  puts("done");
  for (int i = 0; i < height; i++) {
    free(maze[i]);
  }
  free(maze);
  cmdline_parser_free(&args_info);
  exit(EXIT_SUCCESS);
}
