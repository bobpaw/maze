/*
  A simple maze generator using ncurses.
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
#include "main.h"

int main (int argc, char * argv[]) {
  srand(time(NULL) % clock());
  int ch = 0;
  struct gengetopt_args_info args_info;
  int maze_width = 0;
  int maze_height = 0;
  int width = 0;
  int height = 0;
  char floorchar = '.';
  char wallchar = '#';
  char * base_map = NULL;
  int winx = 0;
  int winy = 0;
  int x = 0;
  int y = 0;
  int loser = 1;
  if (cmdline_parser(argc, argv, &args_info) != 0) {
    fprintf(stderr, "Error processing command line arguments\n");
    endwin();
    exit(EXIT_FAILURE);
  }
  initscr();
  raw();
  curs_set(0);
  keypad(stdscr, TRUE);
  noecho();
  if (args_info.width_given) {
    maze_width = (args_info.width_arg < (COLS-1)>>1 ? args_info.width_arg : (COLS-1)>>1);
  } else {
    if (args_info.wall_given) {
      maze_width = COLS;
    } else {
      maze_width = (COLS-1)>>1;
    }
  }
  if (args_info.height_given) {
    maze_height = (args_info.height_arg < (LINES-1)>>1 ? args_info.width_arg : (LINES-1)>>1);
  } else {
    if (args_info.wall_given) {
      maze_height = LINES;
    } else {
      maze_height = (LINES-1)>>1;
    }
  }
  if (args_info.maximize_given) {
    maze_width = (COLS-1) >> 1;
    maze_height = (LINES-1) >> 1;
  }
  int ** maze = NULL;
  if (args_info.wall_given) {
    y = 0;
    maze = carve_passages(maze_width, maze_height, 0, 0, maze);
    width = maze_width * 2 + 1;
    height = maze_height + 1;
    base_map = calloc(width * height + 1, sizeof(char));
    int n = 0;
    base_map[n] = ' ';
    ++n;
    for (int i = 0; i < (maze_width * 2 - 1); i++) {
      base_map[n] = '_';
      ++n;
    }
    base_map[n] = ' ';
    ++n;
    for (int y = 0; y < maze_height; y++) {
      base_map[n] = '|';
      ++n;
      for (int x = 0; x < maze_width; x++) {
        base_map[n] = ((maze[y][x] & S) != 0) ? ' ' : '_';
        ++n;
        if ((maze[y][x] & E) != 0) {
          base_map[n] = (((maze[y][x] | maze[y][x+1]) & S) != 0) ? ' ' : '_';
          ++n;
        } else {
          base_map[n] = '|';
          ++n;
        }
      }
    }
  } else {
    y = 1;
    base_map = genmaze(maze_width, maze_height, &width, &height, wallchar, floorchar, NULL);
  }
  if (base_map == NULL) {
    fprintf(stderr, "Error creating maze\n");
    endwin();
    exit(EXIT_FAILURE);
  }
  while (ch != 'q') {
    erase();
    if (ch == KEY_UP) {
      if (args_info.wall_given && has(maze[y][x], N) || y > 0 && base_map[(y-1)*width+x] == floorchar) --y;
    } else if (ch == KEY_DOWN) {
      if (args_info.wall_given && has(maze[y][x], S) || y < height - 1 && base_map[(y+1)*width+x] == floorchar) y++;
    } else if (ch == KEY_LEFT) {
      if (args_info.wall_given && has(maze[y][x], W) || x > 0 && base_map[y*width+x-1] == floorchar) x--;
    } else if (ch == KEY_RIGHT) {
      if (args_info.wall_given && has(maze[y][x], E) || x < width - 1 && base_map[y*width+x+1] == floorchar) x++;
    }
    for (int i = 0; i < height; ++i) {
      mvaddnstr(i, 0, base_map + (i * width), width);
    }
    mvprintw(height + 1, 0, "(x, y): (%d, %d)", x, y);
    if (args_info.wall_given) {
    mvaddch(y+1, x*2+1, '@');
    mvchgat(y+1, x*2+1, 1, has(maze[y][x], S) ? A_NORMAL : A_UNDERLINE, 0, NULL);
    } else {
    mvaddch(y, x, '@');
    }
    refresh();
    if (args_info.wall_given && x == maze_width - 1 && y == maze_height - 1 || x == width-1 && y == height - 2) {
      loser = 0;
      break;
    }
    ch = getch();
  }
  int starttime = 0;
  if (loser == 0) {
    int stringlength = strlen("#Congratulations. You win!#");
    mvprintw(winy/2-1, winx/2-stringlength/2, "###########################\n");
    mvprintw(winy/2, winx/2-stringlength/2,   "#Congratulations. You win!#\n");
    mvprintw(winy/2+1, winx/2-stringlength/2, "###########################\n");
  } else {
    int stringlength = strlen("#Loser :(#");
    mvprintw(winy/2-1, winx/2-stringlength/2, "##########\n");
    mvprintw(winy/2, winx/2-stringlength/2,   "#Loser :(#\n");
    mvprintw(winy/2+1, winx/2-stringlength/2, "##########\n");
  }
  refresh();
  timeout(-1);
  getch();
  endwin();
  if (loser == 0) {
    printf("Congratulations. You win!\n");
  } else {
    printf("Loser: :(\n");
  }
  free(base_map);
  if (maze != NULL) {
  for (int i = 0; i < maze_height; ++i) {
    free(maze[i]);
  }
  free(maze);
  }
  cmdline_parser_free(&args_info);
  exit(EXIT_SUCCESS);
}
