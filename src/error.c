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

/*=============================================================================+
 |                                  Includes                                   |
 +=============================================================================*/
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "commons.h"
#include "error.h"




/*=============================================================================+
 |                                   Macros                                    |
 +=============================================================================*/
#define JSON_MAX_USER_BUFF 0xFF




/*=============================================================================+
 |                              Global Variables                               |
 +=============================================================================*/
static char user_buffer[JSON_MAX_USER_BUFF];

static const JSON_Error* current = NULL;

static const JSON_Error errors[JSON_ETOTAL+1] =
{
  {JSON_EDICT_NHASH_FUNC,     "JSON_Dict as no hash function associated with it.\n"},
  {JSON_ELIST_FAILED_REALLOC, "JSON_List failed to realloc its size.\n"},
  {JSON_EHASH_NHASHABLE,      "Type is not hashable.\n"},
  {JSON_EDICT_SIZE_EQZ,       "Size of dict hash table is equal to 0.\n"},
  {JSON_ELIST_SIZE_EQZ,       "Size of list vector is equal to 0.\n"},
  {JSON_ELIST_BAD_INDEX,      "Index of list is too large.\n"},
  {JSON_EUSER,                user_buffer},
  {JSON_ETOTAL,               NULL}
};



/*=============================================================================+
 |                          Function Implementations                           |
 +=============================================================================*/
void JSON_ClearError(void)
{
  current = NULL;
}




const char* JSON_GetError(void)
{
  if (JSON_likely(current != NULL))
    return current->message;

  return NULL;
}




JSON_Errors JSON_GetErrorNo(void)
{
  if (JSON_likely(current != NULL))
    return current->errno;

  return -1;
}




int JSON_SetError(const char* format, ...)
{
  int     code;
  va_list args;

  va_start (args, format);

  code = vsprintf(user_buffer, format, args);

  if (code > 0)
    current = &errors[JSON_EUSER];
  else
    JSON_ClearError();

  va_end(args);

  return code;
}




int __JSON_SetError(int errorno)
{
  if (errorno > JSON_ETOTAL)
  {
    fprintf(stderr, "Invalid errorno '%d' passed in __JSON_SetError\n.", errorno);
      return -1;
  }

  if (errorno < 0)
    current = NULL;
  else
    current = &errors[errorno];

  return 0;
}
