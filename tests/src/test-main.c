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

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "parser.h"
#include "json.h"
#include "print.h"

// ANSI color escape
#define ORANGE "\e[38;5;202m"
#define PURPLE "\e[38;5;56m"
#define YELLOW "\e[93m"
#define RESET "\e[0m"

int hashType(const JSON_Type* type, size_t* hash);

int main(int argc, char* argv[])
{
  JSON_Dict* dict = NULL;

  FILE* in = fopen(argv[1], "r");

  struct timespec start;
  struct timespec end;

  clock_gettime(CLOCK_REALTIME, &start);

  if (in)
    JSON_parse(&dict, in, &hashType, 64, 128);

  clock_gettime(CLOCK_REALTIME, &end);

  printf("Bison finished parsing in %lf nanosecond(s).\n",
         (1E9 * end.tv_sec + end.tv_nsec) - (1E9 * start.tv_sec + start.tv_nsec));

  fclose(in);

  if (!dict)
    return 1;

  FILE* out = fopen("out.json", "w");

  if (out)
    JSON_PrintDict(dict, out);

  JSON_FreeDict(dict);

  fclose(out);

  return 0;
}

int hashType(const JSON_Type* type, size_t* hash)
{
  if (type->label == NULL)
  {
    __JSON_SetError(JSON_EHASH_NHASHABLE);
    return -1;
  }

  const char* str = type->label;

  *hash = 0;

  while (*str)
    *hash += (size_t)(*(str++));

  return 0;
}
