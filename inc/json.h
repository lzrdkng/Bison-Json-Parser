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
 * @file json.h
 *
 * @brief All JSON data structure.
 *
 * There's 3 possible structures in JSON. They all have circular
 * dependency.
 *
 * The main data structure is 'JSON_Type'. An instance of this type
 * can have a label attached to it, if it's in the symbol table of a
 * 'JSON_Dict'. It will also have a pointer to the next entry in this
 * symbol table. This structure is the most important one, because it
 * as an annonymous union that can contain all the possible types in
 * JSON, which is 'int, char*, double, JSON_Dict, JSON_List'. There's
 * also an integer in the structure to determine what type is hold in
 * the structure.
 *
 * The second type is the 'JSON_Dict'. It's basicaly a symbol table of
 * 'JSON_Type', that use hash function and bucket. It's possible to
 * assign different hash function to different instance by passing a
 * pointer to the hash function in the 'JSON_MallocDict' function.
 *
 * The third type is the 'JSON_List'. It's a smart structure that
 * contain a list of pointers to different 'JSON_Type'. It act like a
 * smart structure, i.e it will grow automatically by itself when it's
 * full. However, it will not shrink. To do so, the user will have to
 * do it manually.
 *
 */

#ifndef _JSON_H
#define _JSON_H
/*================================= Includes =================================*/
#include "error.h"
#include "dict.h"
#include "list.h"
#include "type.h"
#endif // _JSON_H
