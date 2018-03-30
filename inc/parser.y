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
 * @file parser.y
 *
 * Bison json parser.
 */
/*============================================================================*/
/*================================= Prologue =================================*/
%{
/*============================================================================*/
/*================================= Includes =================================*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "json.h"
#include "parser.h"
/*============================================================================*/
/*================================ Prototypes ================================*/
  int JSON_lex(JSON_STYPE* valP,
                     JSON_LTYPE* locP,
                     FILE* fd);

  int JSON_parse(struct JSON_Dict**,
                 FILE* fd,
                 JSON_Hash hashFunc,
                 size_t dictSize,
                 size_t listSize);

  void JSON_error(JSON_LTYPE* locP,
                  struct JSON_Dict**,
                  FILE* fd,
                  JSON_Hash hashFunc,
                  size_t dictSize,
                  size_t listSize,
                  const char* error);
%}
/*============================================================================*/
/*============================ Bison Definitions =============================*/
%code requires {
#include "json.h"
  typedef int (*JSON_Hash) (const struct JSON_Type*, size_t*);
 }

%union {
  int                bool;
  double             num;
  char*              str;
  struct JSON_Type*  type;
  struct JSON_Dict*  dict;
  struct JSON_List*  list;
}

%define api.prefix {JSON_}
%debug
%locations
%define api.pure full
%error-verbose

%parse-param {struct JSON_Dict** dict}
%parse-param {FILE* fd}
%parse-param {JSON_Hash hashFunc}
%parse-param {size_t dictSize}
%parse-param {size_t listSize}
%lex-param   {FILE* fd}

%token <bool> BOOL
%token <num>  NUM
%token <str>  STR

%type <type> value
%type <list> value-sequence

%type <type> entry
%type <dict> entry-sequence

%type <list> array
%type <dict> object

%destructor {free($$);} STR
%destructor {JSON_FreeDict($$);} object entry-sequence
%destructor {JSON_FreeList($$);} array  value-sequence
%destructor {JSON_FreeType($$);} value  entry
/*================================= Grammar ==================================*/
%%

START:
/*  Empty  */
|
START object
{
  *dict = $2;
}
;

entry:
STR ':' value
{
  $3->label = strdup($1);
  $$ = $3;
  free($1);
}
;

entry-sequence:
entry
{
  $$ = JSON_MallocDict(dictSize, hashFunc);

  if ($$)
    JSON_SetDict($$, $1);
  else
  {
    perror("Failed MallocDict in entry-sequence::entry");
    YYABORT;
  }

}
|
entry-sequence ',' entry
{
  JSON_SetDict($1, $3);
  $$ = $1;
}
;

object:
'{' entry-sequence '}'
{
  $$ = $2;
}
;

array:
'[' value-sequence ']'
{
  $$ = $2;
}
;

value-sequence:
value
{
  $$ = JSON_MallocList(listSize);

  if ($$)
    JSON_PushList($$, $1);
  else
  {
    perror("Failed MallocList in value-sequence::value");
    YYABORT;
  }
}
|
value-sequence ',' value
{
  JSON_PushList($1, $3);
  $$ = $1;
}
;

value:
STR
{
  $$ = JSON_MallocType(NULL, JSON_STRING);

  if ($$)
    $$->str = strdup($1);
  else
  {
    perror("Failed MallocType in value::STR");
    YYABORT;
  }

  free($1);
}
|
NUM
{
  $$ = JSON_MallocType(NULL, JSON_NUMBER);

  if ($$)
    $$->num = $1;
  else
  {
    perror("Failed MallocType in value::NUM");
    YYABORT;
  }
}
|
object
{
  $$ = JSON_MallocType(NULL, JSON_DICT);

  if ($$)
    $$->dict = $1;
  else
  {
    perror("Failed MallocType in value::object");
    YYABORT;

  }
}
|
array
{
  $$ = JSON_MallocType(NULL, JSON_LIST);

  if ($$)
    $$->list = $1;
  else
  {
    perror("Failed MallocType in value::list");
    YYABORT;
  }
}
|
BOOL
{
  $$ = JSON_MallocType(NULL, JSON_BOOLEAN);

  if ($$)
    $$->bool = $1;
  else
  {
    perror("Failed MallocType in value::BOOL");
    YYABORT;
  }
}
;

%%
/*  ================================ Epilogue ================================  */
void JSON_error(JSON_LTYPE* locP,
                struct JSON_Dict** dict,
                FILE* fd,
                JSON_Hash hashFunc,
                size_t dictSize,
                size_t listSize,
                const char* error)
{
  fprintf(stderr, "%s at %d.%d-%d.%d\n",
          error,
          locP->first_line,
          locP->last_line,
          locP->first_column,
          locP->last_column);
}
