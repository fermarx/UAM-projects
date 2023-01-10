#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <limits.h>
#include "types.h"
#include "list.h"
#include "stack_fp.h"
#include "tree.h"

#define MAX_ID 1000

void error(List *l, List *l2, BSTree *t, BSTree *t2){
    if(l) list_free(l);
    if(l2) list_free(l2);
    if (t) tree_destroy(t);
    if(t2) tree_destroy(t2);
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
Status listRandomAndOrdered (int n, List *pl);

Status listOfRandomOrdered (int n, List *pl) {
    int i, x = 0;
    Status st = OK;

    if (!pl) return ERROR;

    for (i=0; i < n && st == OK; i++) {
        x =(rand() % (MAX_ID - x)) + x;
        st = list_pushOrder (pl, &x, 1);
    }

    if (st == ERROR) {
        fprintf (stderr, "%s\n", strerror (errno));
        return ERROR;
    }

    return OK;
}

Status listRandomAndOrdered (int n, List *pl){
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

int main (int argc, char **argv) {

    List *l1 = NULL;
    List *l2 = NULL;
    BSTree *t1 = NULL;
    BSTree *t2 = NULL;
    int n, i, aux, depth;

    if(argc < 2){
        printf("Enter: <./executable> <List length>\n");
        return 1;
    }

    n = atoi(argv[1]);

    l1 = list_new(int_free, int_copy, int_print, int_cmp);
    if (!l1) {
        error(l1, l2, t1, t2);
        return 1;
    }

    l2 = list_new(int_free, int_copy, int_print, int_cmp);
    if(!l2) {
        error(l1, l2, t1, t2);
        return 1;
    }

    t1 = tree_init(int_free, int_copy, int_print, int_cmp);
    if(!t1){
        error(l1, l2, t1, t2);
        return 1;
    }

    t2 = tree_init(int_free, int_copy, int_print, int_cmp);
    if(!t2){
        error(l1, l2, t1, t2);
        return 1;
    }

    if(listRandomAndOrdered(n, l1) == ERROR) {
        error(l1, l2, t1, t2);
        return 1;
    }

    if(listOfRandomOrdered(n, l2) == ERROR) {
        error(l1, l2, t1, t2);
        return 1;
    }

    for(i = 0; i < n; i++){
        aux = *(int*)list_getElementInPos(l1, i);
        if(!aux){
            error(l1, l2, t1, t2);
            return 1;
        }
        if(tree_insert(t1, &aux) == ERROR){
            error(l1, l2, t1, t2);
            return 1;
        }
    }

    for(i = 0; i < n; i++){
        aux = *(int*)list_getElementInPos(l2, i);
         if(!aux){
            error(l1, l2, t1, t2);
            return 1;
        }
        if(tree_insert(t2, &aux) == ERROR){
            error(l1, l2, t1, t2);
            return 1;
        }
    }
    
    printf("--- Printing List: 0 ... \n");
    if(list_print(stdout, l1) == ERROR){
        error(l1, l2, t1, t2);
        return 1;
    }

    depth = tree_depth(t1);
    if (depth == -1){
        error(l1, l2, t1, t2);
        return 1;
    }
    printf("\n\nTree depth: %i", depth);

    printf("\n\nTree postOrder Traversal ...\n");
    tree_postOrder(stdout, t1);
    
    printf("\n\nTree preOrder Traversal ...\n");
    tree_preOrder(stdout, t1);
    
    printf("\n\nTree inOrder Traversal ...\n");
    tree_inOrder(stdout, t1);

    printf("\n\n\n--- Printing List: 1 ... \n");
        if(list_print(stdout, l2) == ERROR){
        error(l1, l2, t1, t2);
        return 1;
    }

    depth = tree_depth(t2);
    if (depth == -1){
        error(l1, l2, t1, t2);
        return 1;
    }
    printf("\n\nTree depth: %i", depth);

    printf("\n\nTree postOrder Traversal ...\n");
    tree_postOrder(stdout, t2);
    
    printf("\n\nTree preOrder Traversal ...\n");
    tree_preOrder(stdout, t2);
    
    printf("\n\nTree inOrder Traversal ...\n");
    tree_inOrder(stdout, t2);

    printf("\n");

    list_free(l1);
    list_free(l2);
    tree_destroy(t1);
    tree_destroy(t2);

    return 0;
}