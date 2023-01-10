#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "queue_fp.h"
#include "node.h"
#include "graph.h"

#define MAXSTRING 65536

void error(Queue *q, Node *n, long *ids, Node *n2){
    if(q) queue_free(q);
    q = NULL;
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


/****
* @brief Implements the BFS algorithm from an initial node
* @param pg, Graph
* @param ini_id, Origin node Id
* @param end_id, Destination node Id
* @param path, String with the traversed node's name.
* This parameter is modified by the function.
* @return, OK or ERROR if any pointer is NULL
****/
Status graph_breadthFirst (Graph *pg, long ini_id, long end_id, char *nodestraversed);

Status graph_breadthFirst (Graph *pg, long ini_id, long end_id, char *nodestraversed){

    Queue *q = NULL;
    Node *n = NULL, *n2 = NULL;
    int i;
    long *ids = NULL;
    long *ids2 = NULL;

    /** B1: Inicializar una cola auxiliar para almacenar nodos. */
    q = queue_init(node_free, node_copy, node_print); 
    if(!q){
        error(q, n, ids, n2);
        return ERROR;
    }
    /** B2: Etiquetar todos los nodos a WHITE excepto el nodo
     * de inicio que se etiquetará BLACK */
    ids = graph_getNodesId(pg);
    if(!ids){
        error(q, n, ids, n2);
        return ERROR;
    }
    for(i = 0; i < graph_getNumberOfNodes(pg); i++){
        n = graph_getNode(pg, ids[i]);
        if(!n){
            error(q, n, ids, n2);
            return ERROR;
        }
        if (ids[i] == ini_id){
            if (node_setLabel(n, BLACK) == ERROR){
                error(q, n, ids, n2);
                return ERROR;
            }
            if(graph_setNode(pg, n) == ERROR){
                error(q, n, ids, n2);
                return ERROR;
            }
        } else {
            if(node_setLabel(n, WHITE) == ERROR){
                error(q, n, ids, n2);
                return ERROR;
            }
            if(graph_setNode(pg, n) == ERROR){
                error(q, n, ids, n2);
                return ERROR;
            }
        }
        node_free(n);
        n = NULL;
    }

    /** B3: Insertar el nodo de inicio en la cola auxiliar */
    n = graph_getNode(pg, ini_id);
    if(!n){
        error(q, n, ids, n2);
        return ERROR;
    }
    if(queue_insert(q, n) == ERROR){
        error(q, n, ids, n2);
        return ERROR;
    }
    node_free(n);
    n = NULL;

    /*B4: Mientras la cola no esté vacía :*/
    while(queue_isEmpty(q) == FALSE){
        /*B4.1: Extraer un nodo de la cola.
        Insertar el nombre del nodo en la cadena de caracteres. */
        n = queue_extract(q);
        if(!n) {
            printf("Error in queue extract\n");
            error(q, n, ids, n2);
            return ERROR;
        }
        strcat(nodestraversed, node_getName(n));
        strcat(nodestraversed, "  ");
        if(!nodestraversed){
            printf("Error in nodestraversed\n");
            error(q, n, ids, n2);
            return ERROR;
        }
        /**B4.2: Si el nodo extraído es el nodo de llegada, salir del bucle*/
        if(node_getId(n) == end_id)
            break;
        
        /*B4.3: Si el nodo extraído no es el nodo de llegada,
        explorar sus nodos adyacentes : */
        ids2 =  (long*)graph_getConnectionsFrom(pg, node_getId(n));
        if(!ids2){
            printf("Error in graph get connections from\n");
            error(q, n, ids, n2);
            return 1;
        }
        for(i = 0; i < graph_getNumberOfConnectionsFrom(pg, node_getId(n)); i++){
            n2 = graph_getNode(pg, (long)ids2[i]);
            if(!n2){
                printf("Error in graph get node\n");
                error(q, n, ids, n2);
                return ERROR;
            }
            /*B4.3.1: Si el nodo adyacente explorado tuviese etiqueta WHITE,
            modificarla a BLACK e insertarlo en la cola */
            if (node_getLabel(n2) == WHITE) {
                if(node_setLabel(n2, BLACK) == ERROR){
                    printf("Error in node set label\n");
                    error(q, n, ids, n2);
                    return 1;
                }
                if(graph_setNode(pg, n2) == ERROR){
                    printf("Error in setting node\n");
                    error(q, n, ids, n2);
                    return ERROR;
                }
                if(queue_insert(q, n2) == ERROR){
                    printf("Error in queue insert\n");
                    error(q, n, ids, n2);
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
    free(ids);
    queue_free(q);

    return OK;
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

    pf = fopen(argv[1], "r");
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

    if(graph_breadthFirst(g, start, dest, node) == ERROR){
        error2(pf, g, node);
        return 1;
    }

    printf("\n%s\n\n", node);

    error2(pf, g, node);

    return 0;
}