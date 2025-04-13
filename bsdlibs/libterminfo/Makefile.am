lib_LIBRARIES = libterminfo.a

libterminfo_a_SOURCES = \
  compile.c \
  curterm.c \
  setupterm.c \
  hash.c \
  term.c \
  termcap.c \
  ti.c \
  tparm.c \
  tputs.c


subdirheadersdir = $(includedir)/

subdirheaders_HEADERS = \
	include/term.h

AM_CFLAGS = -O2 -std=c99 -g0 -I.
AM_CPPFLAGS = \
	-I$(srcdir) \
	-I$(srcdir)/include




