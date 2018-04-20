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
 * @file io.c
 *
 * @brief Implementation of IO JSON services
 */

/*=============================================================================+
 |                                  Includes                                   |
 +=============================================================================*/
#include "io.h"




/*=============================================================================+
 |                                   Macros                                    |
 +=============================================================================*/
#define PRINT_INDENT(fd) for (size_t __j__=0; __j__< 2*nest_level; ++__j__) fprintf(fd, " ")




/*=============================================================================+
 |                              Global Variables                               |
 +=============================================================================*/
static size_t      nest_level = 0;
static const char* bools[3]   = {"false", "null", "true"};




/*=============================================================================+
 |                          Function Implementations                           |
 +=============================================================================*/
void JSON_PrintType(const JSON_Type* type, FILE* fd)
{

  PRINT_INDENT(fd);

  if (type->label)
    fprintf(fd, "\"%s\":", type->label);

  switch (type->type)
  {
  case JSON_BOOLEAN:
    fprintf(fd, "%s", bools[type->bool + 1]);
    break;
  case JSON_NUMBER:
    fprintf(fd, "%lf", type->num);
    break;
  case JSON_STRING:
    fprintf(fd, "\"%s\"", type->str);
    break;
  case JSON_LIST:
    JSON_PrintList(type->list, fd);
    break;
  case JSON_DICT:
    JSON_PrintDict(type->dict, fd);
    break;
  default:
    break;
  }
}




void JSON_PrintList(const JSON_List* list, FILE* fd)
{
  PRINT_INDENT(fd);
  fprintf(fd, "[\n");
  ++nest_level;
  for (size_t i=0; i<list->index; ++i)
  {
    JSON_PrintType(list->elements[i], fd);
      fprintf(fd, ",\n");
  }
  // Change last ",\n" for "\n"
  fseek(fd, -2, SEEK_CUR);
  fprintf(fd, "\n");
  --nest_level;
  PRINT_INDENT(fd);
  fprintf(fd, "]");
}




void JSON_PrintDict(const JSON_Dict* dict, FILE* fd)
{
  PRINT_INDENT(fd);
  fprintf(fd, "{\n");
  ++nest_level;
  for (size_t i=0; i<dict->size; ++i)
  {
    JSON_Type* head = dict->buckets[i];

    while (head)
    {
      JSON_PrintType(head, fd);
      fprintf(fd, ",\n");
      head = head->next;
    }
  }
  // Change last ",\n" for "\n"
  fseek(fd, -2, SEEK_CUR);
  fprintf(fd, "\n");
  --nest_level;
  PRINT_INDENT(fd);
  fprintf(fd, "}");
}
