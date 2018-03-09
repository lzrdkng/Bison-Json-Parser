/*
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
 * @file json.h
 *
 * @brief Various Data Structures interfaces.
 */
#ifndef _JSON_H
#define _JSON_H
/*================================= Includes =================================*/
#include <stdlib.h>
/*=========================== Forward Declarations ===========================*/
struct type;
struct list;
struct dict;
/*================================= Typedefs =================================*/
typedef size_t (*hashFnct) (const char*);
/*================================ Structures ================================*/
typedef enum types
{
  JSON_BOOLEAN,
  JSON_NUMBER,
  JSON_STRING,
  JSON_DICT,
  JSON_LIST,
  NONE
} JSON_Types;
/*============================================================================*/
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
typedef struct dict
{
  hashFnct      hash;
  size_t        size;
  struct type** buckets;
} JSON_Dict;

/**
 * @brief Allocate memory for a 'JSON_Dict'.
 *
 * @param [in] size The size of the dict.
 * @param [in] hash The hash function to use.
 *
 * @return A pointer to the allocated 'JSON_Dict' or NULL on failure.
 */
JSON_Dict* JSON_MallocDict(size_t size, hashFnct hash);

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
typedef struct type
{
  char* label;

  JSON_Types type;

  union
  {
    int          bool;
    double       num;
    char*        str;
    JSON_Dict*   dict;
    struct list* list;
  };

  struct type* next;

} JSON_Type;

/**
 * @brief Allocate memory for a new 'JSON_Type'.
 *
 * @param [in] label The label of the new type.
 *
 * @param [in] type The type to use.
 *
 * @return A pointer to the new type, or NULL on failure.
 */
JSON_Type* JSON_MallocType(const char* label, JSON_Types type_);

/**
 * @brief Procedure that free from memory a 'JSON_Type'.
 *
 * @param [in,out] type The 'JSON_Type' to free from memory.
 */
void JSON_FreeType(JSON_Type* type);
/**
 * @brief Procedure that free from memory the member 'value' of a
 * 'JSON_Type'.
 *
 * @param [in,out] type The 'JSON_Type' to free its 'value' member.
 */
void JSON_FreeTypeValue(JSON_Type* type);
/*============================================================================*/
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
typedef struct list
{
  JSON_Type** elements;
  size_t      size;
  size_t      index;
} JSON_List;

/**
 * @brief Allocate memory for a new 'struc list'.
 *
 * @param [in] size The size of the list.
 *
 * @return The new allocated 'JSON_List' or NULL on failure.
 */
JSON_List* JSON_MallocList(size_t size);

/**
 * @brief Procedure taht free from memory a 'JSON_List'
 *
 * @param [in,out] list The 'struct lsit' to free from memory.
 */
void JSON_FreeList(JSON_List* list);
/*=========================== Function Prototypes ============================*/
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
int JSON_GetDictValue(const JSON_Type* key,
                      const JSON_Dict* dict, JSON_Type** value);
/*============================================================================*/
/**
 * @brief Set the bucket of a dict associated with a key.
 *
 * @param [in] key A pointer to the key to hash by the 'JSON_Dict'
 * 'hash' member.
 *
 * @param [in,out] dict The 'JSON_Dict' to set the bucket to.
 *
 * @param [in] ptr A pointer to the bucket to place in the dict.
 *
 * @return 0 on success, -1 on failure.
 */
int JSON_SetDictValue(JSON_Dict* dict, JSON_Type* ptr);
/*============================================================================*/
int JSON_AppendToList(JSON_List* list, JSON_Type* type);

#endif // _JSON_H
