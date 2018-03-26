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
 * @file dict.c
 *
 * @brief JSON_Dict structure implementations.
 */

/*================================= Includes =================================*/
#include <string.h>
#include "json.h"
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
int JSON_GetDict(const JSON_Type* key, const JSON_Dict* dict, JSON_Type** value)
{
  if (dict->hash == NULL)
    return -1;

  size_t i = dict->hash(key->label)%dict->size;

  for (JSON_Type* p = dict->buckets[i]; p; p = p->next)
  {
    if (strcmp(p->label, key->label) == 0)
    {
      *value = p;
      break;
    }
  }

  return 0;
}
/*============================================================================*/
int JSON_SetDict(JSON_Dict* dict, JSON_Type* value)
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
