#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "stack_fp.h"
#include "stack_types.h"
#include "node.h"

#define MAX_SIZE 6

#define OPERATORS {'+', '-','*', '/', '%', '^'}

void error(Stack *s1, Stack *s2){
    if (s1) stack_free(s1);
    s1 = NULL;
    if (s2) stack_free(s2);
    s2 = NULL;
}

/**
* @brief: Get the postfix expression of an infix expression.
* The infix and postfix strings may not overlap and
* the destination postfix string must be
* large enough to receive the infix expression
* (to be checked before calling this function).
*
* @param in Pointer to the infix expression
* @param suf Pointer to the suffix expression
* @return The function returns OK on success or ERROR
* when any of its parametrs is NULL
**/
Status infix2postfix (char *suf, const char *in);

/* It returns TRUE if the character c is an operator or FALSE otherwise.*/
Bool isOperator(char c);

/*It returns the precedence of operator c*/
int prec(char c);

Status infix2postfix (char *suf, const char *in){

    Bool ret;
    Stack *s = NULL, *stack = NULL;
    int i = 0, j = 0;
    char *ele = NULL;

    if (!suf || !in) return ERROR;

    ret = TRUE;
    s = stack_init(char_free, char_copy, char_print);
    if(!s){
        error(s, stack);
        return ERROR;
    }

    stack = stack_init(char_free, char_copy, char_print);
    if(!stack){
        error(s, stack);
        return ERROR;
    }


    while(in[i] != '\0'){
        /* if the symbol being read is an operator */
        if(isOperator (in[i]) == TRUE){
            while (stack_isEmpty(s) == FALSE && prec(*(char*)stack_top(s)) >= prec(in[i])){
                ele = stack_pop(s);
                if(!ele){
                    error(s, stack);
                    return ERROR;
                }
                suf[j] = *ele;
                j++;
                char_free(ele);
            }
            if(stack_push(s, &in[i]) == ERROR){
                error(s, stack);
                return ERROR;
            }
        }

        /* else if the symbol being read is a left parenthesis */
        else if(in[i] == '('){
            if(stack_push(stack, &in[i]) == ERROR){
                error(s, stack);
                return ERROR;
            }
        }

        /* else if the symbol being read is a right parenthesis */
        else if(in[i] == ')'){
            while(stack_isEmpty(stack) == FALSE && *(char*)stack_top(stack) != '(') {
                ele = stack_pop(s);
                if(!ele){
                    error(s, stack);
                    return ERROR;
                }
                suf[j] = *ele;
                j++;
                char_free(ele);
            }
            /* pop the open paranthesis */
            ele = stack_pop(stack);
            if (!ele){
                error(s, stack);
                return ERROR;
            }    
            char_free(ele);
        }
 
        /* else if the symbol being read is an operand, put it in suffix expression */
        else{
            suf[j] = in[i];
            j++;
        }
        i++;
    }
    

    if (ret == TRUE){
        while(stack_isEmpty(s) == FALSE){
            ele = stack_pop(s);
            if(!ele){
                error(s, stack);
                return ERROR;
            }
            suf[j] = *ele;
            j++;
            char_free(ele);
        }
    }


    stack_free(stack);
    stack = NULL;
    stack_free(s);
    s = NULL;


    return TRUE;
}

Bool isOperator(char c){

    int i;
    char op[MAX_SIZE] = OPERATORS;

    if (!c) return FALSE;

    for (i = 0; i < MAX_SIZE; i++) 
        if (c == op[i]) return TRUE;

    return FALSE;
}

int prec(char c){
   
    if(!c) return 3;

    if (c == '^') return 0;
    else if (c == '/' || c == '*' || c == '%')return 1;
    else if (c == '+' || c == '-') return 2;
    
    return 3;
}


int main(int argc, char **argv){

    char *suf = NULL;

    if (argc < 2){
        printf("Enter ./<executable> \"<infix expression>\"\n");
        return 1;
    }

    suf = (char*)calloc(strlen(argv[1]), sizeof(char*));
    if (!suf) return 1;

    if (infix2postfix(suf, argv[1]) == ERROR){
        free(suf);
        return 1;
    }

    printf("Input:\t%s\n", argv[1]);
    printf("output:\t%s\n", suf);

    free(suf);
    suf = NULL;

    return 0;
}