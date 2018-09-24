#include <stdlib.h>

/**
 * shuffle_int:
 * @array:  integer array
 * @n_memb: number of members
 *
 * Shuffle the first n_memb members of array

 *
 * Returns pointer to modified array
 * Returns NULL if array is null, n_memb is less than 1, or if array ends before n_memb
 **/
int* shuffle_int (int * array, size_t n_memb);

int * shuffle_int (int * array, size_t n_memb) {
  srand(time(NULL));
  if ( array == NULL) {
    return NULL;
  }
  if (n_memb > 1) {
    for (size_t i = 0; i < n_memb; i++) {
      size_t j = i + rand() % (n_memb - i);
      int t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  } else {
    return NULL;
  }
  return array;
}
