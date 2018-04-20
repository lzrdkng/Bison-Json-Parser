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

/*=============================================================================+
 |                            Enums and Structures                             |
 +=============================================================================*/
/**
 * @enum JSON_Errors
 *
 * @brief An enumeration of different errors identifier.
 */
typedef enum JSON_Errors
{
  JSON_EDICT_NHASH_FUNC,     /**< Dict has no hash function */
  JSON_ELIST_FAILED_REALLOC, /**< List failed to realloc */
  JSON_EHASH_NHASHABLE,      /**< Type is not hashable */
  JSON_EDICT_SIZE_EQZ,       /**< Dict has hash table size of 0 */
  JSON_ELIST_SIZE_EQZ,       /**< List has vector of size 0 */
  JSON_ELIST_BAD_INDEX,      /**< List index is invalid */
  JSON_EUSER,                /**< Reserved error for user */
  JSON_ETOTAL                /**< Number of errors */
} JSON_Errors;




/**
 * @struct JSON_Error
 *
 * @brief A structure that contain a JSON_Errors identifier and a
 * pointer to a message.
 *
 * @sa JSON_Errors
 */
typedef struct JSON_Error
{
  JSON_Errors errno;   /**< The error identifier number. */
  const char* message; /**< The message associated with the error. */
} JSON_Error;





/*=============================================================================+
 |                             Function Prototypes                             |
 +=============================================================================*/
/**
 * @brief Retrieved the last error raised.
 * @return The current error message
 */
const char* JSON_GetError(void);




/**
 * @brief Retrived the last error' number raised.
 * @return The current error number.
 */
JSON_Errors JSON_GetErrorNo(void);




/**
 * @brief Reset the last error raised to @b NULL.
 */
void JSON_ClearError(void);




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
 * @note On success, the next call to JSON_GetError() will return the
 * newly created error message. On failure, the next call to
 * JSON_GetError() will return @b NULL. Multiple calls to this
 * function will overwrite previous calls.
 *
 * @warning The user buffer error as a maximum length of
 * MAX_USER_BUFFER characters. If the formated message is longer than
 * that, it will be truncate to fit.
 */
int JSON_SetError(const char* format, ...);




/**
 * @brief Set the error with the corresponding errorno.
 *
 * @param[in] errorno The positive error identifier. If strictly
 * negative, the next call to JSON_GetError() will return @b NULL. The
 * function will fail if errorno is greater than JSON_ETOTAL.
 *
 * @return 0 on success, -1 on failure.
 *
 * @note This function should not be use by the user. Use
 * JSON_SetError instead.*/
int __JSON_SetError(int errorno);
#endif // _JSON_ERROR_H
