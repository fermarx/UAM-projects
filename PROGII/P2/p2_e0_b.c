#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "stack_fp.h"
#include "stack_types.h"
#include "node.h"

void error (Node *n, Stack *s1, Stack *s2){
    if (n) node_free(n);
    if (s1) stack_free(s1);
    if (s2) stack_free(s2);
}

int main(){

    Node *n = NULL;
    Stack *s1 = NULL, *s2 = NULL;
    char *c = NULL;

    n = node_init();
    if (!n) return 1;

    s1 = stack_init(string_free, string_copy, string_print);
    if (!s1) {
        error(n, s1, s2);
        return 1;
    }
    
    s2 = stack_init(node_free, node_copy, node_print);
    if (!s2) {
        error(n, s1, s2);
        return 1;
    }

    if (node_setName(n, "first") == ERROR) {
        error(n, s1, s2);
        return 1;
    }
    
    if (node_setId(n, 111) == ERROR) {
        error(n, s1, s2);
        return 1;
    }

    if (node_setLabel(n, WHITE) == ERROR) {
        error(n, s1, s2);
        return 1;
    }

    if(stack_push(s2, n) == ERROR){
        error(n, s1, s2);
        return 1;
    }

    if (stack_push(s1, node_getName(n)) == ERROR){
        error(n, s1, s2);
        return 1;
    }

    if (node_setName(n, "second") == ERROR) {
        error(n, s1, s2);
        return 1;
    }
    
    if (node_setId(n, 222) == ERROR) {
        error(n, s1, s2);
        return 1;
    }

    if (node_setLabel(n, WHITE) == ERROR) {
        error(n, s1, s2);
        return 1;
    }

    if(stack_push(s2, n) == ERROR){
        error(n, s1, s2);
        return 1;
    }

    if (stack_push(s1, node_getName(n)) == ERROR){
        error(n, s1, s2);
        return 1;
    }

    printf("Print Stack nodes:\n");
    if (stack_print(stdout, s2) < 0 ) {
        error(n, s1, s2);
        return 1;
    }
    
    printf("Print Stack string:\n");
    if (stack_print(stdout, s1) < 0) {
        error(n, s1, s2);
        return 1;
    }

    node_free(n);
    printf("\nPoping nodes...\n");
    while (stack_isEmpty(s2) == FALSE){
        n = stack_pop(s2);
        if (!n){
            error(n, s1, s2);
            return 1;
        }
        node_print(stdout, n);
        node_free(n);
    }

    printf("\nPoping nodes-names...\n");
    while (stack_isEmpty(s1) == FALSE){
        c = stack_pop(s1);
        if (!c){
            error(n, s1, s2);
            return 1;
        }
        string_print(stdout, c);
        string_free(c);
    }

    printf("\n");

    stack_free(s1);
    s1 = NULL;

    stack_free(s2);
    s2 = NULL;

    return 0;
}