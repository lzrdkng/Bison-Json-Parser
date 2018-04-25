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


#ifndef _JSON_UTILS_H
#define _JSON_UTILS_H
/*=============================================================================+
 |                                   Typdefs                                   |
 +=============================================================================*/
typedef struct JSON_Type type;
typedef struct JSON_Dict dict;
typedef struct JSON_List list;
typedef JSON_Hash        hash_t;
/*=============================================================================+
 |                                 Structures                                  |
 +=============================================================================*/
typedef struct parser_option
{
  hash_t fn;
  size_t dsize;
  size_t lsize;
} parser_option;




static size_t dummy_hash(JSON_HashKey key)
{
  size_t hash = 0;

  while (key)
    hash += (size_t)*key++;

  return hash;
}
/*=============================================================================+
 |                               Parser Wrapper                                |
 +=============================================================================*/
int fparse(type** obj_pp,
           FILE* fd,
           parser_option* options)
{
  static hash_t hashFunc = NULL;
  static size_t dictSize = 256;
  static size_t listSize = 256;

  if (options)
  {
    hashFunc = options->fn;
    dictSize = options->dsize;
    listSize = options->lsize;
  }

  return JSON_parse(obj_pp, fd, hashFunc, dictSize, listSize);
}




int sparse(type** obj_pp,
           char* str,
           parser_option* options)
{
  static hash_t hashFunc = dummy_hash;
  static size_t dictSize = 256;
  static size_t listSize = 256;

  if (options)
  {
    hashFunc = options->fn;
    dictSize = options->dsize;
    listSize = options->lsize;
  }

  int retval = 0;
  FILE* fd   = fmemopen(str, strlen(str), "r");

  if (!fd)
  {
    perror("fmemopen failed");
    retval = -1;
  }

  if (JSON_parse(obj_pp, fd, hashFunc, dictSize, listSize))
  {
    fprintf(stderr, "Bison failed parsing string at adress %p\n", str);
    retval = -1;
  }

  fclose(fd);

  return retval;
}
/*=============================================================================+
 |                                List Wrapper                                 |
 +=============================================================================*/
#define lfree(LIST) JSON_FreeList(LIST)
#define linsert(VALUE,INDEX,LIST) JSON_InsertList(VALUE,INDEX,LIST)
#define lprint(LIST, FD) JSON_PrintList(LIST, FD)

/*=============================================================================+
 |                                Type Wrapper                                 |
 +=============================================================================*/
#define tfree(TYPE) JSON_FreeType(TYPE)
#define tbool(PTR,VALUE) \
  JSON_Type* PTR = JSON_MallocType(NULL, JSON_BOOLEAN); \
  if (PTR) PTR->bool = VALUE
#define tnumber(PTR,VALUE) \
  JSON_Type* PTR = JSON_MallocType(NULL, JSON_NUMBER); \
  if (PTR) PTR->num=VALUE
#endif // _JSON_UTILS_H
