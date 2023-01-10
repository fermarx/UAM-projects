/**
 * @brief It defines common types
 *
 * @file types.h
 * @author Profesores PPROG
 * @version 1.0
 * @date 13-01-2015
 * @copyright GNU Public License
 */

#ifndef TYPES_H
#define TYPES_H

#define WORD_SIZE 1000
#define NO_ID -1
/**
 * @brief Definition of the Id as a long number.
 * 
 */
typedef long Id;
/**
 * @brief Enumeration for the use of Bool fucntion which isnt avaible in C programming unless you implement this.
 * 
 */
typedef enum {
  FALSE, TRUE
} BOOL;
/**
 * @brief Enumeration for the correct use of status, instead of returning 0 or 1.
 * 
 */
typedef enum {
  ERROR, OK
} STATUS;
/**
 * @brief Enumeration for the use of directions north, south, east and west.
 * 
 */
typedef enum {
  N, S, E, W
} DIRECTION;

#endif
