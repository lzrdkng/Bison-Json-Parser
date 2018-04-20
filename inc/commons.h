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
 * @file commons.h
 *
 * @brief Commons macros and constants
 */
#ifndef _JSON_COMMONS_H
#define _JSON_COMMONS_H

/*=============================================================================+
 |                                   Macros                                    |
 +=============================================================================*/
#if defined(__GNUC__) || defined(__clang__)
#  define JSON_likely(x)   __builtin_expect(x, 1)
#  define JSON_unlikely(x) __builtin_expect(x, 0)
#else
#  define JSON_likely(x) (x)
#  define JSON_unlikely(x) (x)
#endif /*  defined(__GNUC__) || defined(__clang__)  */


#endif // _JSON_COMMONS_H
