#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "stack_fp.h"
#include "stack_types.h"
#include "node.h"

/**
* @brief: Reverse the words of an input string to an output string.
* The words in the input string are separated by a space character.
* The strings may not overlap, and the destination string strout
* must be large enough to receive the inverted expression.
*
* @param strout, Pointer to the output buffer
* @param strin, Pointer to the input expression
* @return The function returns OK or ERROR
**/

Status reverseWords (char *strout, const char *strin);

Status reverseWords (char *strout, const char *strin){

    Stack *s1 = NULL;
    int i = 0, j = 0;
    char *ele = NULL;

    if (!strout || !strin) return ERROR;

    s1 = stack_init(char_free, char_copy, char_print);
    if (!s1) return ERROR;

    for(i = 0; i <= strlen(strin); i++){
        
        if (strin[i] == ' ' || i == (strlen(strin))){

            for(; stack_isEmpty(s1) == FALSE; j++){
                
                ele = stack_pop(s1);
                if (!ele){
                    stack_free(s1);
                    return ERROR;
                }
                strout[j] = *(char*)ele;
                free(ele);
            }
            strout[j] = ' ';
            j++;

        } else {
            if (stack_push(s1, &strin[i]) == ERROR){
                stack_free(s1);
                return ERROR;
            }
        } 
    }

    stack_free(s1);

    return OK;
}

int main (int argc, char **argv){

    char *strout = NULL;

    if (argc < 2){
        printf ("Enter ./<Executable> \"<phrase>\" \n");
        return 1;
    }

    strout = (char*)calloc(strlen(argv[1]), sizeof(char*));
    if (!strout)return 1;

    if (reverseWords(strout, argv[1]) == ERROR){
        free(strout);
        return 1;
    }

    printf("Input:\t%s\n", argv[1]);
    printf("Output:\t%s\n", strout);

    free(strout);

    return 0;
}