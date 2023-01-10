#include "stack.h"

#define MAXSTACK 1024

struct _Stack {
    int top;
    Element *item[MAXSTACK];
};

Stack * stack_init (){

    Stack *newS = NULL;

    newS = (Stack*) malloc(sizeof(Stack));
    if (!newS) return NULL;

    newS->top = -1;

    return newS;
}

void stack_free(Stack *s){

    while (s->top != -1){
        element_free(s->item[s->top]);
        s->top--;
    }
    free(s);
}

/*NEW memory*/
Status stack_push(Stack *s, const Element *ele){

    Element *cpy = NULL;

    if(!s || !ele || stack_isFull(s) == TRUE) return ERROR;
    
    cpy = element_copy(ele);
    if (!cpy){
        element_free(cpy);
        return ERROR;
    }

    s->top++; 
    s->item[s->top] = cpy;
    if (!s->item[s->top]){
        return ERROR;
    }

    return OK;
}

/*NOT new memory*/
Element * stack_pop(Stack *s){

    Element *rtrn = NULL;

    if(!s || stack_isEmpty(s) == TRUE) return NULL;

    rtrn = s->item[s->top];
    if (!rtrn) return NULL;

    s->item[s->top] = NULL;
    s->top--;

    return rtrn;
}

/*NEW memory*/
Element * stack_top(const Stack *s){

    Element *cpy = NULL;

    if (s == NULL || stack_isEmpty(s) == TRUE) return NULL;

    cpy = element_init();
    if (!cpy) return ERROR;
    
    /*cpy = element_copy(s->item[s->top]);*/
    cpy = s->item[s->top];
    if (!cpy){
        element_free(cpy);
        return NULL;
    }

    return cpy;
}

Bool stack_isEmpty(const Stack *s){

    if (!s) return TRUE;

    if (s->top == -1) return TRUE;

    return FALSE;
}

Bool stack_isFull(const Stack *s){

    if (!s) return FALSE;

    if (s->top == MAXSTACK) return TRUE;

    return FALSE;
}

int stack_print(FILE* pf, const Stack* s){

    int rtrn = 0, i = 0;

    if (!pf || !s) return -1;

    for (i = s->top; i >= 0; i--){
        rtrn += element_print(pf, s->item[i]);
        rtrn += fprintf(pf, "\n");
    }
    
    if (rtrn == -1) return -1;

    return rtrn;
}

int stack_size (const Stack *s) {

    if(!s) return -1;

    return s->top + 1;
}