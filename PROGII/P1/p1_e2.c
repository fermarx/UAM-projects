#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

void error (Node *n1, Node*n2, Graph *g){
    if (n1) node_free(n1);
    if (n2) node_free(n2);
    if (g) graph_free(g);
}

int main() {

    Node *n1 = NULL, *n2 = NULL;
    Graph *g = NULL;

    n1 = node_init();   
    if (!n1){
        error(n1, n2, g);
        return 1;
    } 
    n2 = node_init();
    if (!n2){
        error(n1, n2, g);
        return 1;
    } 
    if (node_setName(n1, "first") == ERROR){
        error(n1, n2, g);
        return 1;
    }
    if (node_setName(n2, "second") == ERROR){
        error(n1, n2, g);
        return 1;
    }
    if (node_setId(n1, 111) == ERROR){
        error(n1, n2, g);
        return 1;
    }
    if (node_setId(n2, 222) == ERROR){
        error(n1, n2, g);
        return 1;
    }

    g = graph_init();
    if(!g){
        error(n1, n2, g);
        return 1;
    }

    printf("Inserting node 1 ... result...: ");
    if (graph_insertNode(g, n1) == OK) {
        printf("1\n");
    } else {
        printf("0\n");
    }

    printf("Inserting node 2 ... result...: ");
    if (graph_insertNode(g, n2) == OK) {
        printf("1\n");
    } else {
        printf("0\n");
    }

    if (graph_insertEdge(g, node_getId(n2), node_getId(n1)) == ERROR){
        error(n1, n2, g);
        return 1;
    }
    printf("Inserting edge: node 2 ---> node 1\n");

    printf("Connected node 1 and node 2? ");
    if (graph_areConnected(g, node_getId(n1), node_getId(n2)) == TRUE) {
        printf("Yes\n");
    } else {
        printf("No\n");
    }

    printf("Connected node 2 and node 1? ");
    if (graph_areConnected(g, node_getId(n2), node_getId(n1)) == TRUE) {
        printf("Yes\n");
    } else {
        printf("No\n");
    }

    printf("Inserting node 2 ... result...: ");
    if (graph_insertNode(g, n2) == OK) {
        printf("1\n");
    } else {
        printf("0\n");
    }

    printf("Graph\n");

	if(graph_print(stdout, g) == -1){
        error(n1, n2, g);
        return 1;
    }

    error(n1, n2, g);

    return 0;
}