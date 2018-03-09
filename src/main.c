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
#include <unistd.h>
#include <stdio.h>
#include "parser.h"
#include "json.h"

#define PRINT_INDENT for (size_t j=0; j<(*nest_level)*2; ++j) printf(" ");

void print_type(struct type* type, size_t* nest_level);
void print_list(struct list* list, size_t* nest_level);
void print_dict(struct dict* dict, size_t* nest_level);

int main(int argc, char* argv[])
{
  JSON_Dict* dict = NULL;

  FILE* in = fopen(argv[1], "r");

  yyparse(&dict, in);

  if (dict)
  {
    size_t nest_level = 0;

    print_dict(dict, &nest_level);
    JSON_FreeDict(dict);
  }

  fclose(in);

  return 0;
}

void print_type(struct type* type, size_t* nest_level)
{
  PRINT_INDENT

  if (type->label)
    printf("\"%s\":", type->label);

  switch (type->type)
  {
  case JSON_BOOLEAN:
    printf("%d", type->bool);
    break;
  case JSON_NUMBER:
    printf("%lf", type->num);
    break;
  case JSON_STRING:
    printf("\"%s\"", type->str);
    break;
  case JSON_LIST:
    print_list(type->list, nest_level);
    break;
  case JSON_DICT:
    print_dict(type->dict, nest_level);
    break;
  default:
    break;
  }
}

void print_list(struct list* list, size_t* nest_level)
{
  printf("\n");
  PRINT_INDENT
  printf("[\n");
  ++(*nest_level);
  for (size_t i=0; i<list->index; ++i)
  {
    print_type(list->elements[i], nest_level);
    printf(",\n");
  }
  --(*nest_level);
  PRINT_INDENT
  printf("]");
}

void print_dict(struct dict* dict, size_t* nest_level)
{
  printf("\n");
  PRINT_INDENT
  printf("{\n");
  ++(*nest_level);
  for (size_t i=0; i<dict->size; ++i)
  {
    struct type* head = dict->buckets[i];

    while (head)
    {
      print_type(head, nest_level);
      printf(",\n");
      head = head->next;
    }
  }
  --(*nest_level);
  PRINT_INDENT
  printf("}");
}
