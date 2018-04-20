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
 * @file io.h
 *
 * @brief Interfaces that provide I/O functionnalities with JSON structures.
 */

#ifndef _JSON_IO_H
#define _JSON_IO_H

/*=============================================================================+
 |                                  Includes                                   |
 +=============================================================================*/
#include <stdio.h>

#include "json.h"




/*=============================================================================+
 |                             Function Prototypes                             |
 +=============================================================================*/
/**
 * @brief Write JSON_Type to file descriptor recursively.
 *
 * @param [in] type The JSON_type to write
 *
 * @param [out] fd The file descriptor to write to
 *
 * @note This function use a global variable to control nesting. It's
 * not threading safe.
 */
void JSON_PrintType(const JSON_Type* type, FILE* fd);




/**
 * @brief Write JSON_List to file descriptor recursively
 *
 * @param [in] list The JSON_List to write
 *
 * @param [out] fd The file descriptor to write to
 *
 * @note This function use a global variable to control nesting. It's
 * not threading safe.
 */
void JSON_PrintList(const JSON_List* list, FILE* fd);




/**
 * @brief Write JSON_Dict to file descriptor recursively.
 *
 * @param [in] dict The JSON_Dict to write
 *
 * @param [out] fd The file descriptor to write to
 *
 * @note This function use a global variable to control nesting. It's
 * not threading safe.
 */
void JSON_PrintDict(const JSON_Dict* dict, FILE* fd);
#endif // _JSON_IO_H
