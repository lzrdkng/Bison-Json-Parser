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
/*================================= Prologue =================================*/
%{
/*================================= Includes =================================*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "json.h"
/*================================== Macros ==================================*/
#define JSON_DICT_SIZE 64
#define JSON_LIST_SIZE 64
/*================================== Extern ==================================*/
/*================================ Prototypes ================================*/
  size_t hashType(const char*);

  int yylex(YYSTYPE* valP, YYLTYPE* locP, FILE* fd);
  int yypase(struct dict**, FILE* fd);
  void yyerror(YYLTYPE* locP, struct dict**, FILE* fd, const char* error);
%}
/*============================ Bison Definitions =============================*/
%union {

  int           bool;
  double        num;
  char*         str;
  struct type*  type;
  struct dict*  dict;
  struct list*  list;
}

%debug
%locations
%pure-parser
%error-verbose

%parse-param {struct dict** dict}
%parse-param {FILE* fd}
%lex-param {FILE* fd}

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

entry:
STR ':' value
{
  $3->label = strdup($1);
  $$ = $3;
  free($1);
}

entry-sequence:
entry
{
  $$ = JSON_MallocDict(JSON_DICT_SIZE, &hashType);

  if ($$)
    JSON_SetDictValue($$, $1);
  else
    perror("Failed MallocDict in entry-sequence::entry");
}
|
entry-sequence ',' entry
{
  JSON_SetDictValue($1, $3);
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

value-sequence:
value
{
  $$ = JSON_MallocList(JSON_LIST_SIZE);

  if ($$)
    JSON_AppendToList($$, $1);
  else
    perror("Failed MallocList in value-sequence::value");
}
|
value-sequence ',' value
{
  JSON_AppendToList($1, $3);
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
    perror("Failed MallocType in value::STR");

  free($1);
}
|
NUM
{
  $$ = JSON_MallocType(NULL, JSON_NUMBER);

  if ($$)
    $$->num = $1;
  else
    perror("Failed MallocType in value::NUM");
}
|
object
{
  $$ = JSON_MallocType(NULL, JSON_DICT);

  if ($$)
    $$->dict = $1;
  else
    perror("Failed MallocType in value::object");
}
|
array
{
  $$ = JSON_MallocType(NULL, JSON_LIST);

  if ($$)
    $$->list = $1;
  else
    perror("Failed MallocType in value::list");
}
|
BOOL
{
  $$ = JSON_MallocType(NULL, JSON_BOOLEAN);

  if ($$)
    $$->bool = $1;
  else
    perror("Failed MallocType in value::BOOL");
}
;

%%
/*  ================================ Epilogue ================================  */
size_t hashType(const char* str)
{
  size_t sum = 0;

  while (*str)
  {
    sum += (int)(*str);
    ++str;
  }

  return sum;
}

void yyerror(YYLTYPE* locP, struct dict** dict, FILE* fd, const char* error)
{
  fprintf(stderr, "%s at %d.%d-%d.%d\n",
          error,
          locP->first_line,
          locP->last_line,
          locP->first_column,
          locP->last_column);
}
