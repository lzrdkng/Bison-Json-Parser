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
#include <JSON/json.h>
#include <JSON/parser.h>
#include <JSON/io.h>

// ANSI color escape
#define ORANGE "\e[38;5;202m"
#define PURPLE "\e[38;5;56m"
#define YELLOW "\e[93m"
#define RESET "\e[0m"

size_t hashType(const char* key);

int main(int argc, char* argv[])
{
  JSON_Type* type = NULL;

  FILE* in = fopen(argv[1], "r");

  if (in)
    JSON_parse(&type, in, &hashType, 64, 128);

  fclose(in);

  if (!type)
    return 1;

  FILE* out = fopen("/dev/null", "w");

  if (out)
    JSON_PrintType(type, out);

  JSON_FreeType(type);

  fclose(out);

  return 0;
}

size_t hashType(const char* key)
{
  size_t hash = 0;

  while (*key)
    hash += (size_t)(*(key++));

  return hash;
}
