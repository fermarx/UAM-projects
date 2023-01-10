#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "stack.h"

#define MAX_ARRAY 3

void error (Stack *s, Element *ele){
    if (s) stack_free(s);
    s = NULL;
    if(ele) element_free(ele);
    ele = NULL;
}

int main(){

    Stack *s1 = NULL;
    char array[MAX_ARRAY] = { 'a', 'b', 'c'}, aux; 
    int i = 0, size = 0, len = 0;
    Element *ele = NULL;

    s1 = stack_init();
    if(!s1){
        error(s1, ele);
        return 1;
    }

    len = sizeof(array) / sizeof(char);

    for (i = 0; i < len ; i++){

        ele = element_init();
        if (!ele) {
            error(s1, ele);
            return 1;
        }

        aux = array[i];
        if(element_setInfo(ele, &aux) == ERROR){
            error(s1, ele);
            return 1;
        }

        if (stack_push(s1, ele) == ERROR){
            error(s1, ele);
            return 1;
        }
        element_free(ele);
    }

    if(stack_print(stdout, s1) < 0){
        error(s1, ele);
        return 1;
    }

    size = stack_size(s1);
    if (size < 0){
        error(s1, ele);
        return 1;
    }
    
    printf("Stack size: %i ", size);

    while (stack_isEmpty(s1) == FALSE) {
        ele = stack_pop (s1);
        if(!ele){
            error(s1, ele);
            return 1;
        }
        printf("Extracted element: ");
        element_print(stdout, ele);
        element_free(ele);
    }

    printf("\n");
    
    size = stack_size(s1);
    if ( size < 0){
        error(s1, ele);
        return 1;
    }

    printf("Stack size: %i\n", size);

    stack_free(s1);
    s1 = NULL;

    return 0;
}