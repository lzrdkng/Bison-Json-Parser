ACLOCAL_AMFLAGS = -I m4

BUILT_SOURCES = parser.h
include_HEADERS = parser.h

AM_YFLAGS = -d

lib_LTLIBRARIES    = libJSON.la

libJSON_la_SOURCES = dict.c \
error.c \
io.c \
lexer.c \
list.c \
type.c \
parser.y

libJSON_la_CPPFLAGS = -I$(top_srcdir)/include
libJSON_la_LDFLAGS = -shared -version-info 1:0:0
