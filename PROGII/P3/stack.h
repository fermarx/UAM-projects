/*
* File: stack.h
* Author: Profesores de PROG2
*/
#ifndef STACK_H
#define STACK_H

#include "element.h"

typedef struct _Stack Stack;


Stack * stack_init ();
void stack_free(Stack *);
Status stack_push(Stack *, const Element *);
Element * stack_pop(Stack *);
Element * stack_top(const Stack *);
Bool stack_isEmpty(const Stack *);
Bool stack_isFull(const Stack *);
int stack_print(FILE*, const Stack*);
int stack_size (const Stack *);

#endif /* STACK_H */