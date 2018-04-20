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
 * @file list.c
 *
 * @brief JSON_List structure implementations.
 */

/*=============================================================================+
 |                                  Includes                                   |
 +=============================================================================*/
#include <string.h>

#include "error.h"
#include "json.h"




/*=============================================================================+
 |                             Function Prototypes                             |
 +=============================================================================*/
JSON_Type* JSON_MallocType(const char* label, JSON_Types type_)
{
  JSON_Type* type = calloc(1, sizeof(JSON_Type));

  if (type)
  {
    if (label)
      type->label = strdup(label);
    else
      type->label = NULL;

    type->type  = type_;
  }

  return type;
}




void JSON_FreeType(JSON_Type* type)
{
  JSON_List* stack = JSON_MallocList(1024);
  JSON_PushList(type, stack);

  JSON_Type*  p;
  JSON_Type* stack_p;

  while ((stack_p = JSON_PopList(stack)) != NULL)
  {
    switch (stack_p->type)
    {
    case JSON_STRING:
      free(stack_p->str);
      break;
    case JSON_DICT:

      for (size_t i = 0; i < stack_p->dict->size; ++i)
      {
        p = stack_p->dict->buckets[i];

        while (p)
        {
          JSON_PushList(p, stack);
          p = p->next;
        }
      }

      free(stack_p->dict->buckets);
      free(stack_p->dict);

      break;
    case JSON_LIST:

      for (size_t i = 0; i < stack_p->list->index; ++i)
      {
        JSON_PushList(stack_p->list->elements[i], stack);
      }

      free(stack_p->list->elements);
      free(stack_p->list);

      break;
    default:
      break;
    }

    free(stack_p->label);
    free(stack_p);
  }

  free(stack->elements);
  free(stack);
}
