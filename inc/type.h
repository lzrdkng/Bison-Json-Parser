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
 * 'JSON_Type' structure definition. An instance of this type
 * can have a label attached to it, if it's in the symbol table of a
 * 'JSON_Dict'. It will also have a pointer to the next entry in this
 * symbol table. This structure is the most important one, because it
 * as an annonymous union that can contain all the possible types in
 * JSON, which is 'int, char*, double, JSON_Dict, JSON_List'. There's
 * also an integer in the structure to determine what type is hold in
 * the structure.
*/

#ifndef _JSON_TYPE_H
#define _JSON_TYPE_H
/*================================= Includes =================================*/
#include <stdlib.h>
/*=========================== Forward Declarations ===========================*/
struct JSON_Dict;
struct JSON_List;
/*================================== Enums ===================================*/
typedef enum JSON_Types
{
  JSON_BOOLEAN,
  JSON_NUMBER,
  JSON_STRING,
  JSON_DICT,
  JSON_LIST,
  NONE
} JSON_Types;
/*================================ Structures ================================*/
/**
 * @struct
 *
 * @brief A structure representing the basic type.
 *
 * @var type::label The label representing the instance in its parent,
 * if any, collection
 *
 * @var type::type A positive integer representing which type is being
 * use.
 *
 * @var type::value The actual value of the type.
 *
 * @var type::next The next type in the collection of its parent.
 *
 * @note A type with labe set to NULL is said to be the root of the
 * overall sub-collections. In this case, the member 'next' should bet
 * set to NULL also.
 */
typedef struct JSON_Type
{
  char* label;

  JSON_Types type;

  union
  {
    int        bool;
    double     num;
    char*      str;
    struct JSON_Dict* dict;
    struct JSON_List* list;
  };

  struct JSON_Type* next;

} JSON_Type;
/*=========================== Function Prototypes ============================*/

/**
 * @brief Allocate memory for a new 'JSON_Type'.
 *
 * @param [in] label The label of the new type.
 *
 * @param [in] type The type to use.
 *
 * @return A pointer to the new type, or NULL on failure.
 */
JSON_Type* JSON_MallocType(const char* label, JSON_Types type);
/*============================================================================*/
/**
 * @brief Procedure that free from memory a 'JSON_Type'.
 *
 * @param [in,out] type The 'JSON_Type' to free from memory.
 */
void JSON_FreeType(struct JSON_Type* type);
/*============================================================================*/
#endif // _JSON_TYPE_H
