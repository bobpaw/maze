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
  int fake_width = 37;
  int fake_height = 19;
  if (argc > 2) {
    fake_width = atoi(argv[1]);
    fake_height = atoi(argv[2]);
  }
  int width = 0;
  int height = 0;
  char floorchar = '.';
  char wallchar = '#';
  char * base_map = NULL;
  int winx = 0;
  int winy = 0;
  int x = 0;
  int y = 1;
  int loser = 1;
  initscr();
  raw();
  curs_set(0);
  keypad(stdscr, TRUE);
  noecho();
  // Insure maze isn't too big
  getmaxyx(stdscr, winy, winx);
  for (;fake_height*2+1 > winy;fake_height--);
  for (;fake_width*2+1 > winx;fake_width--);
  base_map = genmaze(fake_width, fake_height, &width, &height, wallchar, floorchar, NULL);
  if (base_map == NULL) {
    fprintf(stderr, "Error creating maze\n");
    exit(EXIT_FAILURE);
  }
  char * map = NULL;
  map = malloc(height * width+1);
  memset(map, 0, height * width + 1);
  memcpy(map, base_map, height * width);
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
    if (x == width-1 && y == height - 2) {
      loser=0;
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
    mvprintw(winy/2, winx/2-stringlength/2,     "#Loser :(#\n");
    mvprintw(winy/2+1, winx/2-stringlength/2,     "##########\n");
  }
  refresh();
  starttime = time(NULL);
  while (time(NULL) - starttime < 3);
  endwin();
  if (loser == 0) {
    printf("Congratulations. You win!\n");
  } else {
    printf("Loser: :(\n");
  }
  free(base_map);
  free(map);
  exit(EXIT_SUCCESS);
}
