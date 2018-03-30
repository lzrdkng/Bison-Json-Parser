/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/**
 * @file lexer.c
 *
 * Json lexer.
 */
/*================================= Includes =================================*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
/*================================== Macros ==================================*/
#define SKIP_WS(c, fd) while((c = fgetc(fd)) == ' ' || c == '\t')
/*================================ Constants =================================*/
ssize_t getword(char** buf, FILE* fd)
{
  size_t len = 5;

  ssize_t n = 0;

  *buf = calloc(len + 1, sizeof(char));

  if (!*buf)
    return 0;

  char c;

  while (isalnum(c = fgetc(fd)))
  {
    if (n == len)
    {
      len *= 2;
      *buf = realloc(*buf, len + 1);

      if (!*buf)
        return 0;

    }

    (*buf)[n++] = c;
  }

  (*buf)[n] = '\0';

  ungetc(c, fd);

  return n;
}

int JSON_lex(JSON_STYPE* valP, JSON_LTYPE* locP, FILE* fd)
{
  int c;

  SKIP_WS(c, fd)
    ++(locP->last_column);

  locP->first_line = locP->last_line;
  locP->first_column = locP->last_column;

  if (c == EOF)
  {
    if (ferror(fd))
      perror("Lexer EOF");

    return 0;
  }

  if (c == '\n')
  {
    ++(locP->last_line);
    locP->last_column = 0;

    SKIP_WS(c, fd)
      ++(locP->last_column);
  }

  if (c == '"')
  {
    char*  buf = NULL;
    size_t len = 0;

    ssize_t size = getdelim(&buf, &len, '"', fd);

    buf[size - 1] = '\0';

    valP->str = strdup(buf);
    locP->last_column += size;

    free(buf);

    return STR;
  }

  if (isdigit(c) || c == '-')
  {
    double x = 0;

    int n;

    ungetc(c, fd);

    --(locP->last_column);

    fscanf(fd, "%lf%n", &x, &n);

    valP->num = x;

    locP->last_column += n;

    return NUM;
  }

  if (isalpha(c))
  {
    ungetc(c, fd);

    --(locP->last_column);

    char* buf = NULL;

    ssize_t n = getword(&buf, fd);

    if (buf)
    {
      if (strcmp(buf, "true") == 0)
      {
        valP->bool = 1;
        locP->last_column += strlen(buf);
        free(buf);
        return BOOL;
      }
      else if (strcmp(buf, "false") == 0)
      {
        valP->bool = -1;
        locP->last_column += strlen(buf);
        free(buf);
        return BOOL;
      }
      else if (strcmp(buf, "null") == 0)
      {
        valP->bool = 0;
        locP->last_column += strlen(buf);
        free(buf);
        return BOOL;
      }

      for (int i=n; i > 0; --i)
      {
        ungetc(buf[i], fd);
      }

      free(buf);
    }
  }

  return c;
}
