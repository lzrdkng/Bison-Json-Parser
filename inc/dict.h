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
 * @file dict.h
 *
 * 'JSON_Dict' structure definition. It's basicaly a symbol table of
 * 'JSON_Type', that use hash function and bucket. It's possible to
 * assign different hash function to different instance by passing a
 * pointer to the hash function in the 'JSON_MallocDict' function.
 */

#ifndef _JSON_DICT_H
#define _JSON_DICT_H
/*================================= Includes =================================*/
#include <stdlib.h>
/*=========================== Forward Declarations ===========================*/
struct JSON_Type;
struct JSON_List;
/*================================= Typedefs =================================*/
typedef size_t (*hashFnct) (const char*);
/*================================ Structures ================================*/
/**
 * @struct dict
 *
 * @brief A structure that act like a hash table. A dict consists of 4
 * members. A pointer to a procedure, called 'freeBucket' that will
 * free the individual bucket from memory. A pointer to a hash
 * function, called 'hash' that will be called when you try to access
 * an element from the dict. A positive number, called 'size', that
 * represent the number of bucket in the dict. And finally, 'buckets'
 * list of  pointers to individual bucket.
 *
 * @var dict::freeBucket A pointer to the procedure that will free
 * invidiual bucket.
 *
 * @var dict::hash A pointer to the function that will produce the
 * hashed values for the dict.
 *
 * @var dict::size The number of bucket in the dict.
 */
typedef struct JSON_Dict
{
  hashFnct    hash;
  size_t      size;
  struct JSON_Type** buckets;
} JSON_Dict;
/*=========================== Function Prototypes ============================*/
/**
 * @brief Allocate memory for a 'JSON_Dict'.
 *
 * @param [in] size The size of the dict.
 * @param [in] hash The hash function to use.
 *
 * @return A pointer to the allocated 'JSON_Dict' or NULL on failure.
 */
JSON_Dict* JSON_MallocDict(size_t size, hashFnct hash);
/*============================================================================*/
/**
 * @brief Procedure that free from memory a 'JSON_Dict'.
 *
 * @param [in,out] dict The dict to free from memory.
 *
 * @note This procedure will call dict's member 'freeBucket' it it's
 * not NULL.
 */
void JSON_FreeDict(JSON_Dict* dict);
/*============================================================================*/
/**
 * @brief Get the bucket of a dict associated with a key.
 *
 * @param [in] key A pointer to the key to hash by the 'JSON_Dict'
 * 'hash' member.
 *
 * @param [in] dict The 'JSON_Dict' to get the bucket from.
 *
 * @param [out] addr The address of the pointer to fill with the
 * bucket.
 *
 * @return 0 on success, -1 on failure.
 */
int JSON_GetDict(const struct JSON_Type* key,
                 const JSON_Dict* dict, struct JSON_Type** value);
/*============================================================================*/
/**
 * @brief Set a type in the symbol table of a 'JSON_Dict'.
 *
 * @param [in,out] dict The 'JSON_Dict' to modify.
 *
 * @param [in] ptr A pointer to the type to put in the dict.
 *
 * @return 0 on success, -1 on failure.
 */
int JSON_SetDict(JSON_Dict* dict, struct JSON_Type* ptr);
/*============================================================================*/
/**
 * @brief
 *
 * */
int JSON_DelDict(const struct JSON_Type* key, JSON_Dict* dict);
/*============================================================================*/
#endif // _JSON_DICT_H
