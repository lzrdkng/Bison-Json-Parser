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
 * @file error.h
 *
 * @brief Interface to JSON error system.
 */

#ifndef _JSON_ERROR_H
#define _JSON_ERROR_H
/*============================================================================*/
/*============================ Type Declarations =============================*/
/**
 * @enum JSON_Errors
 *
 * @brief An enumeration of different error identifier.
 */
typedef enum JSON_Errors
{
  JSON_EDICT_NHASH_FUNC,
  JSON_ELIST_FAILED_REALLOC,
  JSON_EHASH_NHASHABLE,
  JSON_USER_BUFFER,
  JSON_ETOTAL
} JSON_Errors;
/*============================================================================*/
/**
 * @struct JSON_Error
 *
 * @brief A structure that contain a JSON_Errors identifier and a
 * pointer to a message.
 *
 * @var JSON_Error::errno The error identifier number.
 *
 * @var JSON_Error::message The message associate with the error.
 *
 * @sa JSON_Errors
 */
typedef struct JSON_Error
{
  JSON_Errors errno;
  char*       message;
} JSON_Error;
/*============================================================================*/
/*=========================== Function Prototypes ============================*/
/**
 * @brief Retrieved the last error raised.
 * @return A pointer to the current JSON_Error structure.
 */
const JSON_Error* JSON_GetError();
/*============================================================================*/
/**
 * @brief Reset the pointer returned by JSON_GetError to NULL.
 */
void JSON_ClearError();
/*============================================================================*/
/**
 * @brief Allow the user to set its own error message.
 *
 * @param[in] format The format of the message, same as printf.
 *
 * @param[in] ... Arguments passed to match the format.
 *
 * @return Return the number of character of the new message on
 * success, a negative number on failure.
 *
 * @note On success, the next call to JSON_GetError will return the
 * newly created error. On failure, the next call to JSON_GetError
 * will return NULL. Multiple calls to this function will overwrite
 * previous calls.
 *
 * @warning The user buffer error as a maximum length of MAX_USER_BUFFER
 * characters. If the formated message is longer than that, 'vsprintf'
 * will truncate it to fit the maxium size.
 */
int JSON_SetError(const char* format, ...);
/*============================================================================*/
/**
 * @brief Set the error with the corresponding errorno.
 *
 * @param[in] errorno The positive error identifier. If strictly
 * negative, the next call to JSON_GetError will return NULL. The
 * function will fail if errorno is greater than JSON_ETOTAL.
 *
 * @return 0 on success, -1 on failure.
 *
 * @note This function should not be use by the user. Use
 * JSON_SetError instead.*/
int __JSON_SetError(int errorno);
/*============================================================================*/
#endif // _JSON_ERROR_H
