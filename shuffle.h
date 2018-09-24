/*
    A integer array shuffler
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
  if ( array == NULL) {
    return NULL;
  }
  if (n_memb > 1) {
    for (size_t i = 0; i < n_memb; i++) {
      size_t j = i + random() % (n_memb - i);
      int t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  } else {
    return NULL;
  }
  return array;
}
