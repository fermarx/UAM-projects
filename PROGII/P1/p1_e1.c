#include "node.h"

void error (Node *n1, Node*n2){
    if (n1) node_free(n1);
    if (n2) node_free(n2);
}

int main () {

    Node *n1 = NULL, *n2 = NULL;

    n1 = node_init();
    n2 = node_init();

    if (node_setId(n1, 111) == ERROR){
        error(n1, n2);
        return -1;
    }
    if(node_setId(n2, 222) == ERROR){
        error(n1, n2);
        return -1;
    }

    if (node_setName(n1, "first") == ERROR){
        error(n1, n2);
        return -1;
    }
    if (node_setName(n2, "second") == ERROR){
        error(n1, n2);
        return -1;
    }
    
    if (node_setLabel(n1, WHITE) == ERROR){
        error(n1, n2);
        return -1;
    }
    if(node_setLabel(n2, WHITE) == ERROR){
        error(n1, n2);
        return -1;
    }

    if(node_print(stdout, n1) == -1){
        error(n1, n2);
        return -1;
    }
    if(node_print(stdout, n2) == -1){
        error(n1, n2);
        return -1;
    }

    printf("\n");

    printf("Are they equal? ");
    if (node_cmp(n1, n2) == 0) {
        printf("Yes\n");
    }
    else {
        printf("No\n");
    }

    printf("Id of first node: %li\n", node_getId(n1));

    printf("Name of second node: %s\n", node_getName(n2));

    node_free(n2);
    n2 = node_copy(n1);
    if (!n2){
        error(n1, n2);
        return -1;
    }

    if(node_print(stdout, n1) == -1){
        error(n1, n2);
        return -1;
    }

    if(node_print(stdout, n2) == -1){
        error(n1, n2);
        return -1;
    }    

    printf("\n");

    printf("Are they equal? ");
    if (node_cmp(n1, n2) == 0) {
        printf("Yes\n");
    }
    else {
        printf("No\n");
    }

    node_free(n1);
    node_free(n2);
    
    return 0;
}
