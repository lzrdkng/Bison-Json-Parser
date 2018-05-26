AM_CFLAGS = -ggdb -O0

bin_PROGRAMS = test
test_SOURCES = main.c
test_CPPFLAGS = -I$(top_srcdir)/inc -I../inc
test_LDADD    = -L$(top_srcdir)/lib -lJSON
