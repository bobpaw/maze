AM_OPTIONS = gnu
AM_CFLAGS = -Wall -pedantic -Wextra
bin_PROGRAMS = maze put
maze_SOURCES = main.c cmdline-maze.c maze.c shuffle.c
maze_CFLAGS = -I$(srcdir)
put_SOURCES = put.c maze.c shuffle.c cmdline-put.c
put_CFLAGS = -I$(srcdir)
include_HEADERS = maze.h shuffle.h main.h cmdline-maze.h cmdline-put.h
maintainer-clean-local:
	rm -f *~ \#*#
SUBDIRS = test
