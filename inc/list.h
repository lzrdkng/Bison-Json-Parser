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
/*================================= Includes =================================*/
#include <stdlib.h>
/*=========================== Forward Declarations ===========================*/
struct JSON_Type;
/*================================ Structures ================================*/
/**
 * @struct list
 *
 * @brief A structure act like an array of 'JSON_Type'.
 *
 * @var list::elements A list of type pointer.
 *
 * @var list::size The size of the array.
 *
 * @var list::index The current index of the list.
 */
typedef struct JSON_List
{
  struct JSON_Type** elements;
  size_t      size;
  size_t      index;
} JSON_List;
/*=========================== Function Prototypes ============================*/

/**
 * @brief Allocate memory for a new 'struc list'.
 *
 * @param [in] size The size of the list.
 *
 * @return The new allocated 'JSON_List' or NULL on failure.
 */
JSON_List* JSON_MallocList(size_t size);
/*============================================================================*/
/**
 * @brief Procedure taht free from memory a 'JSON_List'
 *
 * @param [in,out] list The 'struct lsit' to free from memory.
 */
void JSON_FreeList(JSON_List* list);
/*=========================== Function Prototypes ============================*/

/**
 * @brief
 *
 * */
int JSON_InsertList(JSON_List* list, struct JSON_Type* value, int index);
/*============================================================================*/
/**
 * @brief
 *
 * */
int JSON_PushList(JSON_List* list, struct JSON_Type* type);
/*============================================================================*/
/**
 * @brief
 *
 * */
int JSON_PopList(JSON_List* list);
/*============================================================================*/
/**
 * @brief
 *
 * */
int JSON_RemoveList(JSON_List* list, struct JSON_Type** type);
/*============================================================================*/
/**
 * @brief
 *
 * */
int JSON_AtList(JSON_List);
/*============================================================================*/
#endif // _JSON_LIST_H
