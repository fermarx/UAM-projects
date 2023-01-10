#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "tree.h"
#include "node.h"
#include "graph.h"

void error(FILE *pf, Graph* g, BSTree* t, Node *n, long* l){
    if(pf) fclose(pf);
    if(g) graph_free(g);
    if(t) tree_destroy(t);
    if(n) node_free(n);
    if(l) free(l);
    return;
}

/*NO VA BIEN POSTORDER Y PREORDER??????????????????*/

int main(int argc, char **argv){

    Graph *g = NULL;
    FILE *pf = NULL;
    BSTree *t = NULL;
    Node *n = NULL;
    long *nodeId = NULL;
    int i = 0, numNodes;

    if(argc < 2){
        printf("Enter: <./executable> <Graph_name>\n");
        return 1;
    }

    pf = fopen(argv[1], "r");
    if(!pf) return 1;

    g = graph_init();
    if(!g){
        printf("Error initislizing the graph");
        error(pf, g, t, n, nodeId);
        return 1;
    }

    t = tree_init(node_free, node_copy, node_print, node_cmp);
    if(!t){
        printf("Error initislizing the graph");
        error(pf, g, t, n, nodeId);
        return 1;
    }

    /*Lea un grafo.*/
    if(graph_readFromFile(pf,g) == ERROR){
        printf("Error reading the file");
        error(pf, g, t, n, nodeId);
        return 1;
    }

    /*Imprima el grafo en la salida estándar.*/
    printf("Printing graph ...\n");
    if(graph_print(stdout, g) == ERROR){
        printf("Error printing the graph");
        error(pf, g, t, n, nodeId);
        return 1;
    }

    numNodes = graph_getNumberOfNodes(g);

    nodeId = graph_getNodesId(g);
    if(!nodeId){
        printf("Error getting nodes id");
        error(pf, g, t, n, nodeId);
        return 1;
    }

    /*Inserte los nodos en un árbol binario de búsqueda (BST).*/
    for(i = 0; i < numNodes; i++){
        n = graph_getNode(g, nodeId[i]);
        if(!n){
            printf("Error getting the node");
            error(pf, g, t, n, nodeId);
            return 1;
        }
        if(tree_insert(t, n)== ERROR){
            printf("Error inserting the node");
            error(pf, g, t, n, nodeId);
            return 1;
        }
        node_free(n);
    }


    /*Imprima en la salida estándar los nodos obtenidos con los diferentes recorridos del grafo.*/
    printf("\nTree postOrder Traversal ...\n");
    tree_postOrder(stdout, t);
    
    printf("\n\nTree preOrder Traversal ...\n");
    tree_preOrder(stdout, t);
    
    printf("\n\nTree inOrder Traversal ...\n");
    tree_inOrder(stdout, t);

    printf("\n");

    fclose(pf);
    graph_free(g);
    tree_destroy(t);
    free(nodeId);

    return 0;

}