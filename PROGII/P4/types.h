/* 
 * File:   types.h
 * Author: Profesores de PROG2
 */

#ifndef TYPES_H
#define	TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef enum {
    FALSE=0, TRUE=1
} Bool;

typedef enum {
    ERROR=0, OK=1, END=2
} Status;

#endif	/* TYPES_H */

