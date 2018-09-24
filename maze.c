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
#include "maze.h"

int main (int argc, char * argv[]) {
  srand(time(NULL));
  int ch = 0;
  int width = 37;
  int height = 19;
  char floorchar = '.';
  char wallchar = '#';
  char * base_map = NULL;
  base_map = genmaze(width, height, wallchar, floorchar, NULL);
  if (base_map == NULL) {
    fprintf(stderr, "Error creating maze\n");
    exit(EXIT_FAILURE);
  }
  char * map = NULL;
  map = malloc(height * width+1);
  memset(map, 0, height * width + 1);
  memcpy(map, base_map, height * width);
  int x = 0;
  int y = 0;
  initscr();
  raw();
  curs_set(0);
  keypad(stdscr, TRUE);
  noecho();
  while (ch != 'q') {
    erase();
    if (ch == KEY_UP) {
      if (y > 0 && base_map[(y-1)*width+x] == floorchar) y--;
    } else if (ch == KEY_DOWN) {
      if (y < height - 1 && base_map[(y+1)*width+x] == floorchar) y++;
    } else if (ch == KEY_LEFT) {
      if (x > 0 && base_map[y*width+x-1] == floorchar) x--;
    } else if (ch == KEY_RIGHT) {
      if (x < width - 1 && base_map[y*width+x+1] == floorchar) x++;
    }
    memcpy(map, base_map, height * width);
    map[y*width + x] = '@';
    for (int i = 0; i < height; ++i) {
      printw("%.*s\n", width, (map + (i * width)));
    }
    printw("(x, y): (%d, %d)", x, y);
    refresh();
    ch = getch();
  }
  endwin();
  free(base_map);
  free(map);
  exit(EXIT_SUCCESS);
}
