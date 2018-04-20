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
 * @file list.h
 *
 * 'JSON_List' structure definition. It's a smart structure that
 * contain a list of pointers to different 'JSON_Type'. It act like a
 * smart structure, i.e it will grow automatically by itself when it's
 * full. However, it will not shrink. To do so, the user will have to
 * do it manually.
 */

#ifndef _JSON_LIST_H
#define _JSON_LIST_H

/*=============================================================================+
 |                                  Includes                                   |
 +=============================================================================*/
#include <stdlib.h>




/*=============================================================================+
 |                            Forward Declarations                             |
 +=============================================================================*/
struct JSON_Type;




/*=============================================================================+
 |                                 Structures                                  |
 +=============================================================================*/
/**
 * @struct list
 *
 * @brief A structure that act like a vector of JSON_Type.
 *
 * This structure has 3 members:
 *
 * - A list of pointers of JSON_Type, called @b elements. It's the
 *   actual vector.
 *
 * - A positive number, called @b size, that represent the current
 *   size of the vector.
 *
 * - A positive number, called @b index, that represent the current
 *   index in the vector. @e i.e, at what position the next item will
 *   be inserted in.
 *
 * The size of the vector will grow in time. Whenever the index is
 * equal to the size, the size is double to fit more items and realloc
 * is called on the elements member. It's the user responsability to
 * avoid size_t overflow.
 */
typedef struct JSON_List
{
  struct JSON_Type** elements; /**< A list of pointer of JSON_Type. */
  size_t             size;     /**< The current size of the vector */
  size_t             index;    /**< The current index of the vector */
} JSON_List;




/*=============================================================================+
 |                             Function Prototypes                             |
 +=============================================================================*/
/**
 * @brief Allocate memory for a JSON_List.
 *
 * @param [in] size The size of the list.
 *
 * @return A pointer to the allocated JSON_List or @b NULL on
 * failure; more info by calling JSON_GetError().
 */
JSON_List* JSON_MallocList(size_t size);




/**
 * @brief Procedure that free from memory a JSON_List.
 *
 * @param [in,out] list The JSON_List to free from memory.
 */
void JSON_FreeList(JSON_List* list);




/**
 * @brief Insert a value in a list at a certain index.
 *
 * @param [in,out] value The JSON_Type to insert.
 *
 * @param [in] index The index where to insert the value.
 *
 * @param [out] list The JSON_List to insert the value into.
 *
 * @return 0 on success, -1 on failure; more info by calling JSON_GetError().
 */
int JSON_InsertList(struct JSON_Type* value, size_t index, JSON_List* list);




/**
 * @brief Push a value in a list.
 *
 * @param [in,out] value The JSON_Type to push.
 *
 * @param [out] list The JSON_List to push the value into.
 *
 * @return 0 on success, -1 on failure; more info by calling JSON_GetError().
 */
int JSON_PushList(struct JSON_Type* value, JSON_List* list);




/**
 * @brief Return the last element in a list. Remove it also from the
 * vector.
 *
 * @param [in,out] list The list to take the element from.
 *
 * @return The last element in the list if any, @b NULL otherwise.
 *
 * @note The value in the vector is overwrite by @b NULL as a
 * precaution.
 */
JSON_Type* JSON_PopList(JSON_List* list);




/**
 * @brief Return the last element in a list.
 *
 * @param [in] list The list to return the last element from.
 *
 * @return The last element in the list if any, @b NULL otherwise.
 */
const JSON_Type* JSON_TopList(const JSON_List* list);




/**
 * @brief Return an element at an index in a list.
 *
 * @param [in] list The list to return the element from.
 *
 * @param [in] index The index of the element wanted.
 *
 * @return The element at the index on success, @b NULL on failure;
 * see JSON_GetError() for more info.
 */
const JSON_Type* JSON_AtList(const JSON_List* list, size_t index);




/**
 * @brief Resize the size of a list.
 *
 * @param [in] size The new size of the list.
 *
 * @param [out] list The JSON_List to resize.
 *
 * @return 0 on success, -1 on failure; see JSON_GetError() for more
 * info.
 */
int JSON_ResizeList(size_t size, JSON_List* list);
#endif // _JSON_LIST_H
