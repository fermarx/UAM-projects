#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "queue_fp.h"
#include "node.h"
#include "graph.h"

void error (Queue *q, Graph *g, FILE *pf, long *ids){
    if(q) queue_free(q);
    q = NULL;

    if(g) graph_free(g);
    g = NULL;

    if(pf) fclose(pf);
    pf = NULL;

    if(ids) free(ids);
    ids = NULL;

}

int main(int argc, char **argv){

    Queue *q1 = NULL;
    Graph *g1 = NULL;
    Node *n1= NULL;
    FILE *pf = NULL;
    int i = 0;
    long *ids = NULL;

    if(argc < 2){
        printf("Enter <./executable> <file.txt> \n");
        return 1;
    }

    q1 = queue_init(node_free, node_copy, node_print);
    if (!q1){
        printf("Error q1\n");
        return 1;
    }

    g1 = graph_init();
    if (!g1) {
        printf("Error g1\n");
        error(q1, g1, pf, ids);
        return 1;
    }

    pf = fopen(argv[1], "r");
    if(!pf){
        printf("Error pf\n");
        error(q1, g1, pf, ids);
        return 1;
    }

    if (graph_readFromFile(pf, g1) == ERROR){
        printf("Error graph_readFromFile\n");
        error(q1, g1, pf, ids);
        return 1;
    }

    ids = graph_getNodesId(g1);
    if(!ids){
        printf("Error getNodes\n");
        error(q1, g1, pf, ids);
        return 1;
    }

    for(i=0; i < graph_getNumberOfNodes(g1); i++) {
        n1 = graph_getNode(g1, ids[i]);
        if(queue_insert(q1, n1) == ERROR) {
            printf("Error insert\n");
            error(q1, g1, pf, ids);
            return 1;
        }
        node_free(n1);
    }
    
    for(i=0; i < graph_getNumberOfNodes(g1); i++) {
        n1 = graph_getNode(g1, ids[i]);
        if(node_setLabel(n1, BLACK) == ERROR){
            printf("Error set label\n");
            error(q1, g1, pf, ids);
            return 1;
        }
        if (graph_setNode(g1, n1) == ERROR){
            printf("Error set label\n");
            error(q1, g1, pf, ids);
            return 1;
        }
        node_free(n1);
    }
    
    printf("Nodes in graph:\n\n");
    if(graph_print(stdout, g1) == -1){
        printf("Error graph print\n");
        error(q1, g1, pf, ids);
        return 1;
    }

    printf("Queue size: %i\n\n", queue_size(q1));

    printf("Nodes in queue:\n");
    if(queue_print(stdout, q1) == -1){
        printf("Error graph print\n");
        error(q1, g1, pf, ids);
        return 1;
    }
    
    printf ("\n");
    for(i=0; i < graph_getNumberOfNodes(g1); i++) {
        printf ("Extract node:\n");
        n1 = queue_extract(q1);
        if(!n1){
            printf("Error set label\n");
            error(q1, g1, pf, ids);
            return 1;
        }
        node_print(stdout, n1);
        printf ("\n");
        node_free(n1);

    }
    
    printf("Queue size: %i\n", queue_size(q1));

    queue_free(q1);
    graph_free(g1);
    fclose(pf);
    free(ids);


    return 0;
}