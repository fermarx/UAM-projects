#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "node.h"
#include "graph.h"
#include "stack_fp.h"
#include "stack_types.h"

#define MAXSTRING 65536

void error(Stack *s, Node *n, long *ids, Node *n2){
    if(s) stack_free(s);
    s = NULL;
    if(n) node_free(n);
    n = NULL;
    if(ids) free(ids);
    ids = NULL;
    if(n2) node_free(n2);
    n2 = NULL;
    return;
}

void error2(FILE *pf, Graph *g, char* node){
    if(pf) fclose(pf);
    if(g) graph_free(g);
    g = NULL;
    if(node) free(node);
    node = NULL;
}


/**
* @brief This function implements the DFS algorithm starting
* from the node with id ini_id
* @param pg, Graph
* @param ini_id, Origin node Id
* @param end_id, Destination node Id
* @param path, String with the traversed node's name.
* This parameter is modified by the function.
**/
Status graph_deepFirst (Graph *pg, long ini_id, long end_id, char *nodestraversed);

Status graph_deepFirst (Graph *pg, long ini_id, long end_id, char *nodestraversed) {

    Stack *s = NULL;
    Node *n = NULL, *n2 = NULL;
    int i;
    long *ids = NULL;
    int *ids2 = NULL;

    /** B1: Inicializar una cola auxiliar para almacenar nodos. */
    s = stack_init(node_free, node_copy, node_print); 
    if(!s){
        error(s, n, ids, n2);
        return ERROR;
    }
    /** B2: Etiquetar todos los nodos a WHITE excepto el nodo
     * de inicio que se etiquetará BLACK */
    ids = graph_getNodesId(pg);
    if(!ids){
        error(s, n, ids, n2);
        return ERROR;
    }
    for(i = 0; i < graph_getNumberOfNodes(pg); i++){
        n = graph_getNode(pg, ids[i]);
        if(!n){
            error(s, n, ids, n2);
            return ERROR;
        }
        if (ids[i] == ini_id){
            if (node_setLabel(n, BLACK) == ERROR){
                error(s, n, ids, n2);
                return ERROR;
            }
            if(graph_setNode(pg, n) == ERROR){
                error(s, n, ids, n2);
                return ERROR;
            }
        } else {
            if(node_setLabel(n, WHITE) == ERROR){
                error(s, n, ids, n2);
                return ERROR;
            }
            if(graph_setNode(pg, n) == ERROR){
                error(s, n, ids, n2);
                return ERROR;
            }
        }
        node_free(n);
        n = NULL;
    }
    /** B3: Insertar el nodo de inicio en la cola auxiliar */
    n = graph_getNode(pg, ini_id);
    if(!n){
        error(s, n, ids, n2);
        return ERROR;
    }
    if(stack_push(s, n) == ERROR){
        error(s, n, ids, n2);
        return ERROR;
    }
    node_free(n);
    n = NULL;
    /*B4: Mientras la pila no esté vacía :*/
    while(stack_isEmpty(s) == FALSE){
        /*B4.1: Extraer un nodo de la cola.
        Insertar el nombre del nodo en la cadena de caracteres. */
        n = stack_pop(s);
        if(!n) {
            printf("Error in queue extract\n");
            error(s, n, ids, n2);
            return ERROR;
        }
        strcat(nodestraversed, node_getName(n));
        strcat(nodestraversed, "  ");
        if(!nodestraversed){
            printf("Error in nodestraversed\n");
            error(s, n, ids, n2);
            return ERROR;
        }
        /**B4.2: Si el nodo extraído es el nodo de llegada, salir del bucle*/
        if(node_getId(n) == end_id)
            break;
        
        /*B4.3: Si el nodo extraído no es el nodo de llegada,
        explorar sus nodos adyacentes : */
        ids2 =  (int*)graph_getConnectionsFrom(pg, node_getId(n));
        if(!ids2){
            printf("Error in graph get connections from\n");
            error(s, n, ids, n2);
            return 1;
        }
        for(i = 0; i < graph_getNumberOfConnectionsFrom(pg, node_getId(n)); i++){
            n2 = graph_getNode(pg, (long)ids2[i]);
            if(!n2){
                printf("Error in graph get node\n");
                error(s, n, ids, n2);
                return ERROR;
            }
            /*B4.3.1: Si el nodo adyacente explorado tuviese etiqueta WHITE,
            modificarla a BLACK e insertarlo en la cola */
            if (node_getLabel(n2) == WHITE) {
                if(node_setLabel(n2, BLACK) == ERROR){
                    printf("Error in node set label\n");
                    error(s, n, ids, n2);
                    return 1;
                }
                if(node_setPredecessorId(n2, node_getId(n)) == ERROR){
                    printf("Error in node set predecessorId\n");
                    error(s, n, ids, n2);
                    return 1;
                }
                if(graph_setNode(pg, n2) == ERROR){
                    printf("Error in setting node\n");
                    error(s, n, ids, n2);
                    return ERROR;
                }
                if(stack_push(s, n2) == ERROR){
                    printf("Error in stack insert\n");
                    error(s, n, ids, n2);
                    return 1;
                }
            }
            node_free(n2); 
            n2 = NULL;
        }
        free(ids2);
        ids2 = NULL;
        node_free(n);
        n = NULL;        
    }
    /**B5: Liberar recursos*/
    node_free(n);
    free(ids);
    free(ids2);
    stack_free(s);

    return OK;

}

int pathFromTo (FILE *dev, Graph *g, long orignid, long toid);
int pathFromTo_rec (FILE *dev, Graph *g, long originid, long actualid);

int pathFromTo (FILE *dev, Graph *g, long orignid, long toid){
    return pathFromTo_rec(dev, g, orignid, toid);
}

int pathFromTo_rec (FILE *dev, Graph *g, long originid, long actualid){
    
    Node *node = NULL;
    long predid = 0;
    static int c = 0;

    /* Base case */
    if (originid == actualid){
        node = graph_getNode (g, actualid);
        if (!node) return -1;
        c += node_print (dev, node);
        node_free(node);
        return c;
    }
    /* get actual node */
    node = graph_getNode (g, actualid);
    if (!node){
        node_free(node);
        return -1;
    }
    
    /* get predecessor id */
    predid = node_getPredecessorId(node);
    if(predid == -1){
        node_free(node);
        return -1;
    }
    /* recursive call */
    if (pathFromTo_rec (dev, g, originid, predid) == -1)
        return -1;

    /* print actual node */
    c += node_print(dev, node);
    if (c == -1){
        node_free(node);
        return -1;
    }
    node_free(node);
    return c;
}


int main(int argc, char **argv){

    long start, dest;
    Graph *g = NULL;
    char *node = NULL;
    FILE *pf = NULL;

    if(argc < 4){
        printf("Enter: <./executable> <File> <Start node> <Destination node>\n");
        return 1;
    }

    pf = fopen(argv[1], "r");printf("\n");
   
    if(!pf) return 1;
    start = atoi(argv[2]);
    dest = atoi(argv[3]);

    g = graph_init();
    if(!g){
        error2(pf, g, node);
        return 1;
    }

    if(graph_readFromFile(pf,g) == ERROR){
        error2(pf, g, node);
        return 1;
    }

    node = (char*)calloc(MAXSTRING,sizeof(char));
    if(!node){
        error2(pf, g, node);
        return 1;
    }

    if(graph_deepFirst(g, start, dest, node) == ERROR){
        error2(pf, g, node);
        return 1;
    }

    printf("From Origin to End:\n");
    if(pathFromTo(stdout, g, start, dest) == -1){
        error2(pf, g, node);
        return 1;
    }
    
    printf("\n\n");
    error2(pf, g, node);

    return 0;
}
