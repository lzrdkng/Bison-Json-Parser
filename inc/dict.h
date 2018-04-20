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
 * @brief Interfaces to JSON_Dict structure.
 */

#ifndef _JSON_DICT_H
#define _JSON_DICT_H

/*=============================================================================+
 |                                  Includes                                   |
 +=============================================================================*/
#include <stdlib.h>




/*=============================================================================+
 |                                  Typedefs                                   |
 +=============================================================================*/
typedef struct JSON_Type JSON_Type;
typedef struct JSON_List JSON_List;

/** This typedef might change in the futur. It's use by JSON_Dict as
 *  the type to hash for its hash function. */
typedef const char* JSON_HashKey;

/** Prototypes of hash function used by a JSON_Dict. */
typedef size_t (*JSON_HashFunc) (JSON_HashKey);





/*=============================================================================+
 |                                 Structures                                  |
 +=============================================================================*/
/**
 * @struct JSON_Dict
 *
 * @brief A structure that hold a hash table of JSON_Type.
 *
 * The structure has 3 members:
 * - A pointer to a JSON_HashFunc function, called @b hash that
 * will be called when you try to access an element from the dict.
 *
 * - A positive number, called @b size, that represent the number of
 * buckets in the dict.
 *
 * - A pointer to pointers of JSON_Type, @b buckets is a list of
 * JSON_Type. Every entries in the list act as a linked list.
 *
 * A JSON_Dict has a fixed size of buckets. Every bucket entry is a
 * the head of a linked list of JSON_Type. It uses its hash function
 * in order to retrieve the associated bucket. This is why it's
 * important to NOT modify the hash function after the initialization
 * of the structure.
 *
 *
 */
typedef struct JSON_Dict
{
  JSON_HashFunc hash; /**< A pointer to a hash function that will be
                       * used by the hash table. This member should
                       * never be modify directly. Only access it in
                       * order to call it as a regular function.*/

  size_t size; /**< The number of buckets in the hash table.*/

  struct JSON_Type** buckets; /**< The actual hash table implemented
                               * as a fixed list. Every entries in the
                               * list act as a linked list of
                               * JSON_Type.*/
} JSON_Dict;




/*=============================================================================+
 |                             Function Prototypes                             |
 +=============================================================================*/
/**
 * @brief Allocate memory for a JSON_Dict.
 *
 * @param [in] size The size of the dict.
 * @param [in] hash The hash function to use by the hash table.
 *
 * @return A pointer to the allocated JSON_Dict or @b NULL on
 * failure; more info by calling JSON_GetError().
 */
JSON_Dict* JSON_MallocDict(size_t size, JSON_HashFunc hash);




/**
 * @brief Procedure that free from memory a JSON_Dict.
 *
 * @param [in,out] dict The dict to free from memory.
 *
 * @note All entries in the hash table will be free from memory too.
 */
void JSON_FreeDict(JSON_Dict* dict);




/**
 * @brief Get the bucket of a dict associated with a key.
 *
 * @param [in] key The key to hash.
 *
 * @param [in] dict The JSON_Dict to get the bucket from.
 *
 * @return The corresponding bucket, if any. @b NULL otherwise.
 */
const JSON_Type* JSON_GetDictValue(const JSON_HashKey key,
                                   const JSON_Dict* dict);



/**
 * @brief Set a value in the hash table of a JSON_Dict.
 *
 * @param [in,out] dict The JSON_Dict to add the value to.
 *
 * @param [in] value A pointer to the value to add.
 *
 * @return A pointer to the previous value that has been overwrite, if
 * any. @b NULL otherwise.
 *
 * @warning It's the responsability of the user to free from memory,
 * or at least catch the returned pointer, in order to avoid memory
 * leak.
 *
 * @warning It's the user responsability to ensure that the value
 * passed is not already used by another container. @e e.g JSON_List or
 * JSON_Dict. Chances are this will result in @b SEGFAULT.
 */
JSON_Type* JSON_SetDictValue(JSON_Dict* dict, JSON_Type* value);




/**
 * @brief Remove and free from memory a JSON_Type in a JSON_Dict.
 *
 * @param [in] key The key to hash.
 *
 * @param [in,out] dict The JSON_Dict to search in for.
 *
 * @return 0 if a value has been freed. -1 if not.
 */
int JSON_FreeDictValue(const JSON_HashKey key, JSON_Dict* dict);




/**
 * @brief Remove a JSON_Type in a JSON_Dict without freeing it from memory.
 *
 * @param [in] key The key to hash.
 *
 * @param [in,out] dict The JSON_Dict to search in for.
 *
 * @return A Pointer to the removed JSON_Type on success, @b NULL if no
 * value has been removed.
 *
 * @warning It's the responsability of the user to free from memory,
 * or at least catch the returned pointer, in order to avoid memory
 * leak.
 */
JSON_Type* JSON_DelDictValue(const JSON_HashKey key, JSON_Dict* dict);




/**
 * @brief Move a JSON_Type in a source JSON_Dict to a destination JSON_Dict.
 *
 * @param [in] key The key to hash in src.
 *
 * @param [in,out] src The source JSON_Dict to search in for.
 *
 * @param [out] dst The destination JSON_Dict to transfert to.
 *
 * @param [out] x The success status, or @b NULL. On return, x will be
 * 0 if a transfert has been made, or -1 if no entry was found in src.
 *
 * @return A pointer to the previous value in dst that has been
 * overwrite, if any. @b NULL otherwise.
 *
 * @warning It's the responsability of the user to free from memory,
 * or at least catch the returned pointer, in order to avoid memory
 * leak.
 */
JSON_Type* JSON_MvDictValue(const JSON_HashKey key,
                            JSON_Dict* src,
                            JSON_Dict* dst,
                            int* x);
#endif // _JSON_DICT_H
