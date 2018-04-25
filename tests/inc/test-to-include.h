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

#ifndef _JSON_TEST_TO_INCLUDE_H
#define _JSON_TEST_TO_INCLUDE_H

#include "json.h"
#include "parser.h"


/*=============================================================================+
 |                                   Macros                                    |
 +=============================================================================*/
#define TEST(NAME, ...) {NAME, __VA_ARGS__}






/*=============================================================================+
 |                               Includes Tests                                |
 +=============================================================================*/
#include "test-list.h"




JSON_Tester tests[] =
{
  TEST(test_list2),
  TEST(Test_InsertList),
  TEST(test_list3),
  {NULL}
};
#endif // _JSON_TEST_TO_INCLUDE_H
