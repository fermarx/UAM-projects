#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <limits.h>
#include "types.h"
#include "list.h"
#include "stack_fp.h"

#define MAX_ID 1000

void error(Stack *s, Stack *s2, List *l, List *l2){
    if(s) stack_free(s);
    s = NULL;
    if(s2) stack_free(s2);
    s2 = NULL;
    if(l) list_free(l);
    l = NULL;
    if(l2) list_free(l2);
    l2 = NULL;
    return;
}

/**
* @brief This functión insert n random integers between 0 and MAX_ID into
* an ordered list.
*
* This functions modifies the list input parameter.
*
* @param n, Length of the list
* @param ppl, Address of object list
* @return This function returns OK or ERROR.
**/
Status listOfRandomOrdered (int n, List *pl);

Status listOfRandomOrdered (int n, List *pl) {
    int i, x;
    Status st = OK;

    if (!pl) return ERROR;

    for (i=0; i < n && st == OK; i++) {
        x = rand () % MAX_ID ;
        st = list_pushOrder (pl, &x, 1);
    }

    if (st == ERROR) {
        fprintf (stderr, "%s\n", strerror (errno));
        return ERROR;
    }

    return OK;
}

/* Type definition. Pointer function to compare two elements */
typedef int (*f_cmp_type)(const void *, const void*);

/**
* @brief This function concatenate two increased ordered lists
*
* This functions does not modified the ordered input lists l1 and l2.
*
* @parameter l1, Pointer to one list
* @parameter l2, Pointer to the second list
* @parameter lout, Pointer to the output concatenated list
* @parameter fun, Function used to compare the list elements
*
3
* @return This function returns OK or ERROR.
**/
Status listMergeOrdered (List *l1, List *l2, List *lout, f_cmp_type fun);

Status listMergeOrdered (List *l1, List *l2, List *lout, f_cmp_type fun) {

    Stack *s1 = NULL, *s2 = NULL, *saux = NULL;
    List *laux = NULL;
    int *ele = NULL;
    
    if(!l1 || !l2 || !lout){
        printf("Error in init\n");
        return ERROR;
    }

    s1 = stack_init(int_free, int_copy, int_print);
    if (!s1) {
        printf("Error in stack init 1\n");
        error(s1, s2, l1, l2);
        return ERROR;
    }

    s2 = stack_init(int_free, int_copy, int_print);
    if (!s2) {
        printf("Error in stack init 2\n");
        error(s1, s2, l1, l2);
        return ERROR;
    }

    while (list_isEmpty(l1) == FALSE) {
        ele = list_popFront(l1);
        if (!ele) {	
            printf("Error in list pop front 1\n");
            error(s1, s2, l1, l2);
            return ERROR;
        } 
        
        if(stack_push(s1, ele) == ERROR) {
            printf("Error in stack push s1\n");
            error(s1, s2, l1, l2);
            return ERROR;
        }
        int_free(ele);
    }

    while (list_isEmpty(l2) == FALSE) {
        ele = list_popFront(l2);
        if (!ele) {
            printf("Error in list pop front 2\n");
            error(s1, s2, l1, l2);
            return ERROR;
        }

        if(stack_push(s2, ele) == ERROR) {
            printf("Error in stack push s2\n");
            error(s1, s2, l1, l2);
            return ERROR;
        }

        int_free(ele);
    }

    while (stack_isEmpty(s1) == FALSE && stack_isEmpty(s2) == FALSE) {
        if(fun(stack_top(s1), stack_top(s2)) > 0) {
            ele = stack_pop(s1);
            if(!ele) {
                printf("Error in stack pop1\n");
                error(s1, s2, l1, l2);
                return ERROR;
            }
            if(list_pushFront(l1, ele) == ERROR) {
                printf("Error in list push2\n");
                error(s1, s2, l1, l2);
                return ERROR;
            }
        }
        else {
            ele = stack_pop(s2);
            if (!ele) {
                printf("Error in stack pop2\n");
                error(s1, s2, l1, l2);
                return ERROR;
            }
            if (list_pushFront(l2, ele) == ERROR) {
                printf("Error in list push2\n");
                error(s1, s2, l1, l2);
                return ERROR;
            }
        }
        
        if(list_pushFront(lout, ele) == ERROR) {
            printf("Error in list push endd\n");
            error(s1, s2, l1, l2);
            return ERROR;
        }
        int_free(ele);
    }

    if (stack_isEmpty(s1) == TRUE) {
        saux=s2;
        laux=l1;
    }else {
        saux=s1;
        laux=l1;
    }

    while (stack_isEmpty(saux) == FALSE) {
        ele = stack_pop(saux);
        if (!ele) {
            error(s1, s2, l1, l2);
            return ERROR;
        }
        if(list_pushFront(laux, ele) == ERROR) {
            error(s1, s2, l1, l2);
            return ERROR;
        }
        if(list_pushFront(lout, ele) == ERROR) {
            error(s1, s2, l1, l2);
            return ERROR;
        }
        int_free(ele);
    }

    stack_free(s1);
    stack_free(s2);

    return OK;

}


int main (int argc, char **argv) {

    List *l1 = NULL;
    List *l2 = NULL;
    List *lout = NULL;
    int n;

    if(argc < 2){
        printf("Enter: <./executable> <Destination node>\n");
        return 1;
    }

    n = atoi(argv[1]);

    l1 = list_new(int_free, int_copy, int_print, int_cmp);
    if (!l1) {
        printf("Error creating list 1");
        list_free(l1);
        return 1;
    }

    l2 = list_new(int_free, int_copy, int_print, int_cmp);
    if(!l2) {
        printf("Error creating list 2");
        list_free(l2);
        list_free(l1);
        return 1;
    }

    lout = list_new(int_free, int_copy, int_print, int_cmp);
    if(!lout) {
        printf("Error creating list 2");
        list_free(l2);
        list_free(l1);
        return 1;
    }

    if(listOfRandomOrdered(n, l1) == ERROR) {
        list_free(l1);
        return 1;
    }

    if(listOfRandomOrdered(n, l2) == ERROR) {
        list_free(l2);
        return 1;
    }
    
    printf("List 0: \n");
    list_print(stdout, l1);
    printf(" size: %d\n", list_size(l1));

    printf("List 1: \n");
    list_print(stdout, l2);
    printf(" size: %d\n", list_size(l2));

    if(listMergeOrdered(l1, l2, lout, int_cmp) == ERROR) {
        list_free(l1);
        list_free(l2);
        return 1;
    }

    printf("List 2: \n");
    list_print(stdout, lout);
    printf(" size: %d", list_size(lout));


    list_free(l1);
    list_free(l2);
    list_free(lout);

    return 0;
}