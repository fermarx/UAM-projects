#include "types.h"
#include "graph.h"
#include "errno.h"
#include "list.h"
#include "stack_fp.h"
#include "stack_types.h"

#define MAX_LINE 256
#define NO_FILE_POS_VALUES 3
#define NO_FILE_POS_VALUES2 2
#define MAX_NODES 1064

struct _Graph {
    List *plnode; /*!<List with the graph nodes */
    Bool connections[MAX_NODES][MAX_NODES]; /*!<Adjacency matrix */
    int num_nodes; /*!<Total number of nodes in the graph */
    int num_edges; /*!<Total number of connections in the graph */
} ;

/* PRIVATE FUNCTIONS */
int find_node_index(const Graph * g, long nId1);
int* graph_getConnectionsIndex (const Graph * g, int index);

/* It returns the index of the node with id nId1 */
int find_node_index(const Graph * g, long nId1) {
    int i;
    if (!g) return -1;
    for(i=0; i < g->num_nodes; i++) {
        if (node_getId (list_getElementInPos(g->plnode, i)) == nId1) return i;
    }
    /* ID not found*/
    return -1;
}

/* It returns an array with the indices of the nodes connected to the node
 * whose index is index
 * It also allocates memory for the array */
int* graph_getConnectionsIndex(const Graph * g, int index) {
    int *array = NULL, i, j = 0, size;
    if (!g) return NULL;
    if (index < 0 || index >g->num_nodes) return NULL;
    /* get memory for the array */
    size = node_getConnect (list_getElementInPos(g->plnode, index));
    array = (int *) malloc(sizeof (int) * size);
    if (!array) {
        /* print error message */
        fprintf (stderr, "%s\n", strerror(errno));
        return NULL;
    }
    /* assign values to the array with the indices of the connected nodes*/
    for(i = 0; i< g->num_nodes; i++) {
        if (g->connections[g->num_nodes - 1 - index][g->num_nodes - 1 - i] == TRUE) {
            array[j++] = i;
        }
    }
    return array;
}

/* PUBLIC FUNCTIONS */

Graph * graph_init (){

    int i, j;
    Graph *graphini = NULL;

    graphini = (Graph*)malloc(sizeof(Graph));
    if (!graphini) return NULL;

    graphini->plnode = list_new(node_free, node_copy, node_print, node_cmp);
    if(!graphini->plnode) return NULL;
    
    for (i = 0; i < MAX_NODES; i++){
        for (j = 0; j < MAX_NODES; j++){
            graphini->connections[i][j] = FALSE;
        }
    }
    graphini->num_edges = 0;
    graphini->num_nodes = 0;

    return graphini;
}

void graph_free (Graph *g){
    if(g){
        if(g->plnode)
            list_free(g->plnode);
        free(g);
    }
    return;
}

Status graph_insertNode (Graph *g, const Node *n){
    
    int i, id;
    Node *cpy =NULL;

    if(!g || !n || g->num_nodes + 1 == MAX_NODES) return ERROR;

    id = node_getId(n);
    if (find_node_index(g, id) >= 0) return ERROR;

    cpy = node_copy(n);
    if (!cpy) return ERROR;

    if(list_pushFront (g->plnode, cpy) == ERROR) return ERROR;

    if(!node_setNConnect(cpy, 0)) return ERROR;
    g->num_nodes++;
    for (i = 0; i < g->num_nodes; i++) {
        g->connections[g->num_nodes][i] = FALSE;
        g->connections[i][g->num_nodes] = FALSE;
    }

    node_free(cpy);

    return OK;
}

Status graph_insertEdge (Graph *g, const long nId1, const long nId2){

    int i1, i2, n;
    Node *node = NULL;

    if (!g || nId1 < 0 || nId2 < 0) return ERROR;

    i1 = find_node_index(g, nId1);
    if (i1 == -1) return ERROR;
    i2 = find_node_index(g, nId2);
    if (i2 == -1) return ERROR;

    if(g->connections[g->num_nodes - 1 - i1][g->num_nodes - 1 - i2] == FALSE){
        g->connections[g->num_nodes - 1 - i1][g->num_nodes - 1 - i2] = TRUE;
        node = list_getElementInPos(g->plnode, i1);
        n = node_getConnect ( node )+ 1;
        node_setNConnect( node, n);
        g->num_edges++;
    }

    return OK;
}

Node *graph_getNode (const Graph *g, long nId){
    
    Node *ret = NULL, *node = NULL;
    int nIdx;

    if (!g || nId < 0) return NULL;

    nIdx = find_node_index(g, nId);
    if (nIdx == -1) return NULL;

    node = list_getElementInPos(g->plnode, nIdx);

    ret = node_copy(node);

    return ret;
}

Status graph_setNode (Graph *g, const Node *n){

    int nId;
    Node *node = NULL, *aux = NULL;

    if (!g || !n) return ERROR;

    nId = find_node_index(g, node_getId(n));
    if (nId == -1) return ERROR;

    aux = node_copy(n);
    if(!aux)return ERROR;

    node = list_getElementInPos(g->plnode, nId);
    if(!node) return ERROR;

    node_setId(node, node_getId(aux));
    node_setName(node, node_getName(aux));
    node_setNConnect(node, node_getConnect(aux));
    node_setLabel(node, node_getLabel(aux));
    node_setPredecessorId(node, node_getPredecessorId(aux));

    node_free(aux);

    return OK;
}

long * graph_getNodesId (const Graph *g){

    long *array = NULL;
    int i, contt = 0;
    Node *node = NULL;
    
    if (!g) return NULL;
    
    array = (long*) malloc(sizeof(long) * g->num_nodes);
    if (!array) {
        fprintf (stderr, "%s\n", strerror(errno));
        return NULL;
    }
    
    for(i = g->num_nodes - 1; i >= 0; i--){
        node = list_getElementInPos(g->plnode, i);
        array[contt++] = node_getId(node); 
    }
    
    return array;
}

int graph_getNumberOfNodes (const Graph *g){
    
    if (!g) return -1;

    return g->num_nodes;
}

int graph_getNumberOfEdges (const Graph *g){

    if (!g) return -1;

    return g->num_edges;
}

Bool graph_areConnected (const Graph *g, const long nId1, const long nId2){

    int idx1, idx2;

    if(!g || nId1 < 0 || nId2 < 0) return FALSE;

    idx1 = find_node_index(g, nId1);
    if(idx1 == -1) return FALSE;

    idx2 = find_node_index(g, nId2);
    if(idx2 == -1) return FALSE;

    if (g->connections[g->num_nodes - 1 - idx1][g->num_nodes - 1 - idx2] == TRUE) return TRUE;

    return FALSE;
}

int graph_getNumberOfConnectionsFrom (const Graph *g, const long fromId){

    int nId;

    if(!g || fromId < 0) return -1;

    nId = find_node_index(g, fromId);
    if(nId == -1) return -1; 

    return node_getConnect(list_getElementInPos(g->plnode, nId));
}

long* graph_getConnectionsFrom (const Graph *g, const long fromId){

    int nIdx, i, j = 0;
    long *nIndex = NULL;
    Node *node = NULL;;

    if(!g || fromId < 0) return NULL;

    nIdx = find_node_index(g, fromId);
    if(nIdx == -1) return NULL;

    nIndex = (long*)malloc(sizeof(long) * graph_getNumberOfConnectionsFrom(g, fromId));
    if(!nIndex) return NULL;

    for(i = 0; i < g->num_nodes; i++){
        if(g->connections[g->num_nodes - 1 - nIdx][i] == TRUE){
            node = list_getElementInPos(g->plnode, g->num_nodes - 1 - i);    
            nIndex[j] = node_getId(node);    
            j++;
        }
    }

    return nIndex;
}

int graph_print (FILE *pf, const Graph *g) {

    int connect, id, n = 0;
    int i, j;
    long  *graph = NULL;

    if (!pf || !g){
        fprintf (stderr, "%s\n", strerror(errno));
        return -1;
    }

    for (i = 0; i < g->num_nodes; i++) {

        n = n + node_print(pf, list_getElementInPos(g->plnode, i));

        connect = node_getConnect(list_getElementInPos(g->plnode, i));

        id = node_getId(list_getElementInPos(g->plnode, i));
        
        graph = graph_getConnectionsFrom(g, id);
        if (!graph){
            fprintf(stdout, "%i ", id);
            return -1;
        }

        for (j = 0; j < connect; j++) {
            n = n + fprintf(pf, " %li", graph[j]);
        }

        n = n + fprintf(pf, "\n");

        free(graph);
    }

    return n;
}

Status graph_readFromFile (FILE *fin, Graph *g) {
   
    Node *n = NULL;
    char aux[MAX_LINE], name[MAX_LINE];
    int i, nnodes = 0, id1, id2;
    long lab;
    Status aux2 = ERROR;
   
    if(fgets(aux, MAX_LINE, fin) != NULL) 
        if( sscanf(aux, "%d", &nnodes) != 1) return ERROR;
    
    n = node_init();
    if(!n) return ERROR;

    for(i=0; i < nnodes; i++) {
        if(fgets(aux, MAX_LINE, fin) != NULL) {
            if (sscanf(aux, "%d %s %li", &id1, name, &lab) != NO_FILE_POS_VALUES){
                break;
            }
        }
        node_setId (n, id1);
        node_setName (n, name);
        node_setLabel(n, lab);

        if(graph_insertNode (g, n) == ERROR) break;
    }
    
    if(i < nnodes) {
        node_free(n);
        return ERROR;
    }
    
    while(fgets(aux, MAX_LINE, fin)){
        if(sscanf(aux, "%d %d", &id1, &id2) == NO_FILE_POS_VALUES2){
            if (graph_insertEdge(g, id1, id2) == ERROR) break;
        }
    }
    
    if(feof(fin)) aux2 = OK;
    
    node_free(n);

    return aux2;
}