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
 * @file json.c
 *
 * @brief Data Structures implementations.
 */
/*================================= Includes =================================*/
#include <string.h>
#include "json.h"
/*=================================== Enum ===================================*/

/*========================= Function Implementations =========================*/
JSON_Dict* JSON_MallocDict(size_t size, hashFnct hash)
{
  JSON_Dict* dict = malloc(sizeof(JSON_Dict));

  if (dict)
  {
    dict->hash = hash;
    dict->size = size;

    dict->buckets = calloc(dict->size, sizeof(JSON_Type*));
  }

  return dict;
}
/*============================================================================*/
void JSON_FreeDict(JSON_Dict* dict)
{
  if (dict)
  {
    for (size_t i=0; i < dict->size; ++i)
    {
      JSON_Type** headP = &dict->buckets[i];
      JSON_Type*  head  = *headP;

      while (head)
      {
        *headP = head->next;
        JSON_FreeType(head);
        head = *headP;
      }
    }

    free(dict->buckets);
  }

  free(dict);
}
/*============================================================================*/
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
/*============================================================================*/
void JSON_FreeType(JSON_Type* type)
{
  if (type)
  {
    free(type->label);
    JSON_FreeTypeValue(type);
  }

  free(type);
}
/*============================================================================*/
void JSON_FreeTypeValue(JSON_Type* type)
{
  switch (type->type)
  {
  case JSON_BOOLEAN:
  case JSON_NUMBER:
    break;
  case JSON_STRING:
    free(type->str);
    break;
  case JSON_DICT:
    JSON_FreeDict(type->dict);
    break;
  case JSON_LIST:
    JSON_FreeList(type->list);
    break;
  default:
    break;
  }
}
/*============================================================================*/
JSON_List* JSON_MallocList(size_t size)
{
  JSON_List* list = malloc(sizeof(JSON_List));

  if (list)
  {
    list->size  = size;
    list->index = 0;

    list->elements = calloc(size, sizeof(JSON_Type*));
  }

  return list;
}
/*============================================================================*/
void JSON_FreeList(JSON_List* list)
{
  for (size_t i = 0; i < list->size; ++i)
    JSON_FreeType(list->elements[i]);

  free(list->elements);
  free(list);
}
/*============================================================================*/
int JSON_GetDictValue(const JSON_Type* key, const JSON_Dict* dict, JSON_Type** value)
{
  if (dict->hash == NULL)
    return -1;

  size_t i = dict->hash(key->label)%dict->size;

  for (JSON_Type* p = dict->buckets[i]; p; p = p->next)
    if (strcmp(p->label, key->label) == 0)
    {
      *value = p;
      break;
    }

  return 0;
}
/*============================================================================*/
int JSON_SetDictValue(JSON_Dict* dict, JSON_Type* value)
{
  if (dict->hash == NULL)
    return -1;

  size_t i = dict->hash(value->label)%dict->size;

  JSON_Type** headP = &(dict->buckets[i]);
  JSON_Type*  head  = *headP;

  while (head)
  {
    if (strcmp(head->label, value->label) == 0)
    {
      value->next = head->next;
      *headP = value;
      JSON_FreeType(head);
      return 0;
    }

    headP = &(head->next);
    head  = *headP;
  }

  *headP = value;
  return 0;
}
/*============================================================================*/
int JSON_AppendToList(JSON_List* list, JSON_Type* type)
{
  if (list->index == list->size)
  {
    list->size *= 2;
    list->elements = realloc(list->elements, list->size);

    if (!list->elements)
      return -1;
  }

  list->elements[(list->index)++] = type;

  return 0;
}
