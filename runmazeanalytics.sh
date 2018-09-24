#!/bin/sh

if [ $1 ]; then
  for i in $(seq -s' ' $1); do
    ./put
  done
else
  for i in 1 2 3 4 5; do
    ./put
  done
fi | perl maze_analytics.pl
