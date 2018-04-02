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
#include <string.h>
#include "json.h"
/*========================= Function Implementations =========================*/
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
    free(type);
  }
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
