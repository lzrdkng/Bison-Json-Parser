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

/*================================= Includes =================================*/
#include "json.h"
/*========================= Function Implementations =========================*/
JSON_List* JSON_MallocList(size_t size)
{
  JSON_List* list = calloc(1, sizeof(JSON_List));

  if (list)
  {
    list->size     = size;
    list->elements = calloc(size, sizeof(JSON_Type*));
  }

  return list;
}
/*============================================================================*/
void JSON_FreeList(JSON_List* list)
{
  if (list->elements)
  {
    for (size_t i = 0; i < list->size; ++i)
    {
      JSON_Type* ptr = list->elements[i];

      JSON_FreeType(ptr);

    }

    free(list->elements);
  }

  free(list);
}

/*============================================================================*/
int JSON_PushList(JSON_List* list, JSON_Type* type)
{
  if (list->index == list->size)
  {
    list->size *= 2;
    list->elements = realloc(list->elements, sizeof(struct JSON_Type*) * list->size);

    if (!list->elements)
    {
      __JSON_SetError(JSON_ELIST_FAILED_REALLOC);
      return -1;
    }
  }

  list->elements[list->index++] = type;

  return 0;
}
/*============================================================================*/
int JSON_InsertList(JSON_List* list, JSON_Type* value, size_t index)
{
  if (index > list->index)
    return -1;

  if (list->index == list->size)
  {
    list->size *= 2;
    list->elements = realloc(list->elements, sizeof(JSON_Type*) * list->size);

    if (!list->elements)
    {
      __JSON_SetError(JSON_ELIST_FAILED_REALLOC);
      return -1;
    }
  }

  JSON_Type** pp;
  JSON_Type*  p;

  for (size_t i=index; i <= list->index; ++i)
  {
    pp    = &list->elements[i];
    p     = *pp;
    *pp   = value;
    value = p;
  }

  ++list->index;

  return 0;
}
/*============================================================================*/
JSON_Type* JSON_PopList(JSON_List* list)
{
  if (list->index != 0)
    return list->elements[list->index--];

  return NULL;
}
/*============================================================================*/
JSON_Type* JSON_TopList(JSON_List* list)
{
  return list->elements[list->index];
}
/*============================================================================*/
JSON_Type* JSON_AtList(JSON_List* list, size_t index)
{
  if (index < list->index)
    return list->elements[index];

  return NULL;
}
/*============================================================================*/
