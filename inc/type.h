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
 * @file type.h
 *
 * @brief Interfaces to JSON_Type structure.
 */

#ifndef _JSON_TYPE_H
#define _JSON_TYPE_H

/*=============================================================================+
 |                                  Includes                                   |
 +=============================================================================*/
#include <stdlib.h>




/*=============================================================================+
 |                            Forward Declarations                             |
 +=============================================================================*/
struct JSON_Dict;
struct JSON_List;




/*=============================================================================+
 |                                    Enums                                    |
 +=============================================================================*/
/**
 * @enum JSON_Types
 *
 * @brief Enumerate all possible different types in JSON
 */
typedef enum JSON_Types
{
  JSON_BOOLEAN, /**< int between -1, 0 or 1*/
  JSON_NUMBER,  /**< double */
  JSON_STRING,  /**< char* */
  JSON_DICT,    /**< JSON_Dict */
  JSON_LIST,    /**< JSON_List*/
  JSON_NONE
} JSON_Types;




/*=============================================================================+
 |                                 Structures                                  |
 +=============================================================================*/
/**
 * @struct JSON_Type
 *
 * @brief A structure representing the basic type in JSON.
 *
 * The structure has 4 members.
 *
 * - A string, called @b label, that represent the key passed to hash
 *   function, if the JSON_Type is in a JSON_Dict.
 *
 * - A JSON_Types, called @b type, that represent which type is stored
 *   in the anonymous union.
 *
 * - A anonymous union, that can be one of the diffrent types defined
 *   by JSON_Types, excepted JSON_NONE.
 *
 * - A pointer to a JSON_Type, called @b next, that represented the
 *   the next JSON_Type, if any, in the linked list, if the instance
 *   is in a JSON_Dict.
 */
typedef struct JSON_Type
{
  char* label; /**< The label representing the instance, if in a JSON_Dict. */

  JSON_Types type; /**< Integer representing which type is hold in the
                    * annonymous union*/
  union
  {
    int bool;
    double num;
    char* str;
    struct JSON_Dict* dict;
    struct JSON_List* list;
  }; /**< Annonymous union */

  struct JSON_Type* next; /**< The next JSON_Type in the linked list,
                           * if in a JSON_Dict. */
} JSON_Type;




/*=============================================================================+
 |                             Function Prototypes                             |
 +=============================================================================*/
/**
 * @brief Allocate memory for a JSON_Type.
 *
 * @param [in] label The label of the JSON_Type.
 *
 * @param [in] type The JSON_Types to use.
 *
 * @return A pointer to the new type, or NULL on failure; see
 * JSON_GetError() for mor info.
 */
JSON_Type* JSON_MallocType(const char* label, JSON_Types type);




/**
 * @brief Procedure that free from memory a JSON_Type.
 *
 * @param [in,out] type The JSON_Type to free from memory.
 *
 * @note This will also free all other JSON_Type that are hold by the
 * instance.
 */
void JSON_FreeType(struct JSON_Type* type);
#endif // _JSON_TYPE_H
