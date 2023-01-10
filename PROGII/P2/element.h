/*
* File: element.h
* Author: Profesores de PROG2
*/
#ifndef ELEMENT_H
#define ELEMENT_H

#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Element Element;
Element * element_init();
void element_free(Element *);
Element * element_setInfo(Element *, void*);
void * element_getInfo(Element *);
Element * element_copy(const Element *);
Bool element_equals(const Element *, const Element *);
int element_print(FILE *, const Element *);

#endif /* ELESTACK_H */