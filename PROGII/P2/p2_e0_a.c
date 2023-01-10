#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "stack_fp.h"
#include "stack_types.h"

void error (Stack *s){
    if (s) stack_free(s);
    s = NULL;
}

int main(){

    Stack *s1 = NULL;
    char array[] = { 'a', 'b', 'c'}; 
    int i = 0, size = 0, len = 0;
    char *ele = NULL;

    s1 = stack_init(char_free, char_copy, char_print);
    if(!s1){
        error(s1);
        return 1;
    }

    len = sizeof(array) / sizeof(char);

    for (i = 0; i < len; i++){
        if (stack_push(s1, array + i) == ERROR){
            error(s1);
            return 1;
        }
    }

    if(stack_print(stdout, s1) < 0){
        error(s1);
        return 1;
    }

    size = stack_size(s1);
    if (size < 0){
        error(s1);
        return 1;
    }
    
    printf("Stack size: %i\n", size);

    while (stack_isEmpty(s1) == FALSE) {
        ele = stack_pop (s1);
        if(!ele){
            error(s1);
            return 1;
        }
        printf("Extracted element: ");
        char_print (stdout, ele);
        printf ("\n");
        char_free (ele);
    }
    
    size = stack_size(s1);
    if ( size < 0){
        error(s1);
        return 1;
    }

    printf("Stack size: %i\n", size);

    stack_free(s1);
    s1 = NULL;

    return 0;
}