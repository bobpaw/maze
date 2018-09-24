#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

char * genmaze (int width, int height, char wallchar, char floorchar) {
  srand(time(NULL));
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
  ret = malloc(width*height+1);
  return ret;
}
