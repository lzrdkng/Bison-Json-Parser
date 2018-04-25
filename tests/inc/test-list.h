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
 * @file test-list.h
 *
 * @brief All Tests for JSON_List structure.
 */

#ifndef _JSON_TEST_LIST_H
#define _JSON_TEST_LIST_H

/*=============================================================================+
 |                                  Includes                                   |
 +=============================================================================*/
#include <stdlib.h>

#include "json.h"
#include "io.h"
#include "utils.h"
#include "test-struct.h"







/*=============================================================================+
 |                                    Tests                                    |
 +=============================================================================*/
void* Test_InsertList(void* arg)
{
  static char data[] = "[1,2,3]";
  static char buff[1024];

  type* t = NULL;
  list* l = NULL;

  /*  Write fo buffer  */
  FILE* fd = fmemopen(buff, sizeof(buff), "w");


  INIT_WORKER(val, "InsertList", buff, 1);

  if (sparse(&t, data, NULL))
  {
    val->ok = 0;
    return val;
  }

  /*  Number to insert  */
  tnumber(A, -1);
  tnumber(B, -2);
  tnumber(C, -3);

  l = t->list;

  /*  Initial state of the list  */
  lprint(l,fd);

  /*  Insert at begining  */
  linsert(A,0,l);
  lprint(l,fd);

  /*  Insert at end  */
  linsert(B,l->index,l);
  lprint(l,fd);

  /*  Insert in middle  */
  linsert(B,2,l);
  lprint(l,fd);

  /*  Check final list  [A,1,C,2,3,B] */

  tfree(t);

  fclose(fd);

  return val;
}

void* test_list2(void* arg)
{
  static const char name[] = "Test List 2";
  static const char buff[] = "\0";

  JSON_WorkerRetVal* val = calloc(1, sizeof(JSON_WorkerRetVal));

  val->ok    = 1;
  val->name  = name;
  val->buff  = buff;

  return val;
}

void* test_list3(void* arg)
{
  static const char name[] = "Test List 3";
  static const char buff[] = "\0";

  JSON_WorkerRetVal* val = calloc(1, sizeof(JSON_WorkerRetVal));

  val->ok   = 1;
  val->name = name;
  val->buff = buff;

  return val;
}

#endif // _JSON_TEST_LIST_H
