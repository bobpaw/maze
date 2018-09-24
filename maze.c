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
