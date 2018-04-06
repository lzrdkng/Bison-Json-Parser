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
JSON_Dict* JSON_MallocDict(size_t size, JSON_HashFunc hash)
{
  if (size == 0)
  {
    __JSON_SetError(JSON_EDICT_SIZE_EQZ);
    return NULL;
  }

  if (hash == NULL)
  {
    __JSON_SetError(JSON_EDICT_NHASH_FUNC);
    return NULL;
  }

  JSON_Dict* dict = calloc(1, sizeof(JSON_Dict));

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
    if (dict->buckets)
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
}
/*============================================================================*/
const JSON_Type* JSON_GetDictValue(const char* key,
                                   const JSON_Dict* dict)
{
  size_t i       = dict->hash(key) % dict->size;
  JSON_Type* ptr = dict->buckets[i];

  while (ptr)
  {
    if (strcmp(ptr->label, key) == 0)
      break;

    ptr = ptr->next;
  }

  return ptr;;
}
/*============================================================================*/
JSON_Type* JSON_SetDictValue(JSON_Dict* dict, JSON_Type* value)
{
  size_t i = dict->hash(value->label) % dict->size;

  JSON_Type** headP = &(dict->buckets[i]);
  JSON_Type*  head  = *headP;

  while (head)
  {
    if (strcmp(head->label, value->label) == 0)
    {
      value->next = head->next;
      *headP      = value;

      break;
    }

    headP = &(head->next);
    head  = *headP;
  }

  *headP = value;

  return head;
}
/*============================================================================*/
int JSON_DelDictValue(const char* key, JSON_Dict* dict)
{
  size_t i = dict->hash(key) % dict->size;

  JSON_Type** headP = &(dict->buckets[i]);
  JSON_Type*  head  = *headP;

  while (head)
  {
    if (strcmp(head->label, key))
    {
      *headP = head->next;
      JSON_FreeType(head);

      return 0;
    }

    headP = &head->next;
    head  = *headP;
  }

  return -1;
}
/*============================================================================*/
JSON_Type* JSON_RmDictValue(const char* key, JSON_Dict* dict)
{
  size_t i = dict->hash(key) % dict->size;

  JSON_Type** headP = &dict->buckets[i];
  JSON_Type*  head  = *headP;

  while (head)
  {
    if (strcmp(head->label, key) == 0 )
    {
      *headP = head->next;

      break;
    }

    headP = &head->next;
    head  = *headP;
  }

  return head;
}
/*============================================================================*/
JSON_Type* JSON_MvDictValue(const char* key, JSON_Dict* src, JSON_Dict* dst)
{
  JSON_Type* ptr = JSON_RmDictValue(key, src);

  if (ptr == NULL)
    return NULL;

  return JSON_SetDictValue(dst, ptr);
}
/*============================================================================*/
