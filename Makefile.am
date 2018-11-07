AM_OPTIONS = gnu
AM_CFLAGS = -Wall -pedantic
bin_PROGRAMS = maze put
maze_SOURCES = main.c cmdline-maze.c maze.c shuffle.c
maze_CFLAGS = -I$(srcdir)
put_SOURCES = put.c maze.c shuffle.c
put_CFLAGS = -I$(srcdir)
include_HEADERS = maze.h shuffle.h main.h cmdline-maze.h
maintainer-clean-local:
	rm -f *~ \#*#
