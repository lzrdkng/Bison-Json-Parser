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

  int JSON_parse(struct JSON_Type**,
                 FILE* fd,
                 JSON_Hash hashFunc,
                 size_t dictSize,
                 size_t listSize);

  void JSON_error(JSON_LTYPE* locP,
                  struct JSON_Type**,
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
  typedef size_t (*JSON_Hash) (const char*);
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

%parse-param {struct JSON_Type** type}
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
  *type = JSON_MallocType(NULL, JSON_DICT);

  if (*type)
  {
    (*type)->dict = $2;
  }
  else
  {
    perror(JSON_GetError());
    YYABORT;
  }
}
|
START array
{
  *type = JSON_MallocType(NULL, JSON_LIST);

  if (*type)
  {
    (*type)->list = $2;
  }
  else
  {
    perror(JSON_GetError());
    YYABORT;
  }
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
    JSON_SetDictValue($$, $1); // Can't have overwriten value
  else
  {
    perror(JSON_GetError());
    YYABORT;
  }

}
|
entry-sequence ',' entry
{
  JSON_Type* ow = JSON_SetDictValue($1, $3);

  JSON_FreeType(ow);

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
  {
    if (JSON_PushList($$, $1) != 0)
    {
      fprintf(stderr, "%s", JSON_GetError());
      YYABORT;
    }
  }
  else
  {
    perror(JSON_GetError());
    YYABORT;
  }
}
|
value-sequence ',' value
{
  if (JSON_PushList($1, $3) != 0)
  {
    fprintf(stderr, "%s", JSON_GetError());
    YYABORT;
  }

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
    perror(JSON_GetError());
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
    perror(JSON_GetError());
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
    perror(JSON_GetError());
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
    perror(JSON_GetError());
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
    perror(JSON_GetError());
    YYABORT;
  }
}
;
%%
/*  ================================ Epilogue ================================  */
void JSON_error(JSON_LTYPE* locP,
                struct JSON_Type** type,
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
