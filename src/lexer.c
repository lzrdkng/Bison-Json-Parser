/*
 * Copyright (C) Olivier Dion <olivier.dion@polymtl.ca>
 *
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
 * @brief JSON lexer.
 */

/*=============================================================================+
 |                                  Includes                                   |
 +=============================================================================*/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commons.h"
#include "parser.h"




/*=============================================================================+
 |                                   Macros                                    |
 +=============================================================================*/
#define SKIP_WS(c, fd) while((c = fgetc(fd)) == ' ' || c == '\t')



/*=============================================================================+
 |                          Function Implementations                           |
 +=============================================================================*/
/**
 * @brief Read a word from a stream.
 *
 * @param [out] buf The buffer to fill.
 *
 * @param [in] fd The stream to read from
 *
 * @return The number of characters readed from stream.
 */
static ssize_t get_word(char** buf, FILE* fd)
{
  /*  Initial size of buffer is 255 */
  size_t len = 0xFF;
  ssize_t n  = 0;
  char c;

  /*  Buffer allocaiton  */
  *buf = calloc(len + 1, sizeof(char));

  if (JSON_unlikely(*buf == NULL))
    return 0;

  /* While readed char is alnum, place it in the buffer  */
  while (isalnum(c = fgetc(fd)))
  {
    /*  Resize the buffer if needed  */
    if (n == len)
    {
      len *= 2;
      *buf = realloc(*buf, len + 1);

      if (JSON_unlikely(*buf == NULL))
        return 0;
    }

    (*buf)[n++] = c;
  }

  /*  End of string  */
  (*buf)[n] = '\0';

  /*  Push back last readed char  */
  ungetc(c, fd);

  return n - 1;
}




/**
 * @brief JSON Lexer.
 *
 * @param [out] val_p Pointer to the value used by JSON_parser
 *
 * @param [out] loc_p Pointer to the location used by JSON_Parser
 *
 * @param [in] fd File descriptor to read from
 *
 * @return Token type.
 */
int JSON_lex(JSON_STYPE* val_p, JSON_LTYPE* loc_p, FILE* fd)
{
  int c;

  SKIP_WS(c, fd)
    ++(loc_p->last_column);

  loc_p->first_line = loc_p->last_line;
  loc_p->first_column = loc_p->last_column;

  if (c == EOF)
  {
    if (ferror(fd))
      perror("Lexer EOF");

    return 0;
  }

  if (c == '\n')
  {
    ++(loc_p->last_line);
    loc_p->last_column = 0;

    SKIP_WS(c, fd)
      ++(loc_p->last_column);
  }

  if (c == '"')
  {
    char*  buf = NULL;
    size_t len = 0;

    ssize_t size = getdelim(&buf, &len, '"', fd);

    buf[size - 1] = '\0';

    val_p->str = strdup(buf);
    loc_p->last_column += size;

    free(buf);

    return STR;
  }

  if (isdigit(c) || c == '-')
  {
    double x = 0;

    int n;

    ungetc(c, fd);

    --(loc_p->last_column);

    fscanf(fd, "%lf%n", &x, &n);

    val_p->num = x;

    loc_p->last_column += n;

    return NUM;
  }

  if (isalpha(c))
  {
    ungetc(c, fd);

    --(loc_p->last_column);

    char* buf = NULL;

    ssize_t n = get_word(&buf, fd);

    if (n)
    {
      if (strcmp(buf, "true") == 0)
      {
        val_p->bool = 1;
        loc_p->last_column += n;
        free(buf);
        return BOOL;
      }
      else if (strcmp(buf, "false") == 0)
      {
        val_p->bool = -1;
        loc_p->last_column += n;
        free(buf);
        return BOOL;
      }
      else if (strcmp(buf, "null") == 0)
      {
        val_p->bool = 0;
        loc_p->last_column += n;
        free(buf);
        return BOOL;
      }

      for (int i=n-1; i >= 0; --i)
      {
        ungetc(buf[i], fd);
      }

      free(buf);
    }
  }

  return c;
}
