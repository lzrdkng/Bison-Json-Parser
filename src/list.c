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
#include "commons.h"
#include "error.h"
#include "json.h"





/*=============================================================================+
 |                                   Macros                                    |
 +=============================================================================*/
#define RESIZE_LIST(SIZE,LIST) \
  (LIST)->size = (SIZE); \
  (LIST)->elements = realloc((LIST)->elements, sizeof(struct JSON_Type*) * (LIST)->size); \
  if (JSON_unlikely(!(LIST)->elements)) \
  { \
    __JSON_SetError(JSON_ELIST_FAILED_REALLOC); \
    return -1; \
  } \

/*=============================================================================+
 |                          Function Implementations                           |
 +=============================================================================*/
JSON_List* JSON_MallocList(size_t size)
{
  if (size == 0)
  {
    __JSON_SetError(JSON_ELIST_SIZE_EQZ);
    return NULL;
  }

  JSON_List* list = calloc(1, sizeof(JSON_List));

  if (JSON_likely(list != NULL))
  {
    list->size     = size;
    list->elements = calloc(size, sizeof(JSON_Type*));
  }

  return list;
}




void JSON_FreeList(JSON_List* list)
{
  if (JSON_likely(list != NULL))
  {
    /*  Free Vector  */
    if (JSON_likely(list->elements != NULL))
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
}




int JSON_PushList(JSON_Type* type, JSON_List* list)
{
  /*  Resize if needed */
  if (JSON_unlikely(list->index == list->size))
  {
    RESIZE_LIST(list->size * 2, list);
  }

  /*  Push  */
  list->elements[list->index++] = type;

  return 0;
}




int JSON_InsertList(JSON_Type* value, size_t index,  JSON_List* list)
{
  /*  Check if valid index  */
  if (index > list->index)
  {
    __JSON_SetError(JSON_ELIST_BAD_INDEX);
    return -1;
  }

  /*  Resize if needed  */
  if (JSON_unlikely(list->index == list->size))
  {
    RESIZE_LIST(list->size * 2, list)
  }

  /*  Insert the value  */
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




JSON_Type* JSON_PopList(JSON_List* list)
{
  if (JSON_unlikely(list->index == 0))
    return NULL;

  JSON_Type** pp = &list->elements[--list->index];
  JSON_Type*  p  = *pp;

  *pp = NULL;

  return p;
}




const JSON_Type* JSON_TopList(const JSON_List* list)
{
  if (JSON_unlikely(list->index == 0))
    return NULL;

  return list->elements[list->index - 1];
}




const JSON_Type* JSON_AtList(const JSON_List* list, size_t index)
{
  if (JSON_unlikely(index >= list->index))
  {
    __JSON_SetError(JSON_ELIST_BAD_INDEX);
    return NULL;
  }

  return list->elements[index];
}




int JSON_ResizeList(size_t size, JSON_List* list)
{
  if (JSON_unlikely(size <= list->index))
  {
    __JSON_SetError(JSON_ELIST_BAD_INDEX);
    return -1;
  }

  /*  Resize list  */
  RESIZE_LIST(size, list)

  return 0;
}
