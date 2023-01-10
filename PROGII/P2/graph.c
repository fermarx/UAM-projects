#include "types.h"
#include "graph.h"

#define MAX_NODES 4096
#define MAX_LINE 256
#define NO_FILE_POS_VALUES 2

struct _Graph {
    Node *nodes[MAX_NODES]; /*!<Array with the graph nodes */
    Bool connections [MAX_NODES][MAX_NODES]; /*!<Adjacency matrix */
    int num_nodes; /*!<Total number of nodes in the graph */
    int num_edges; /*!<Total number of connection in the graph*/
};

/* PRIVATE FUNCTIONS */
int find_node_index(const Graph * g, long nId1);
int* graph_getConnectionsIndex (const Graph * g, int index);

/* It returns the index of the node with id nId1 */
int find_node_index(const Graph * g, long nId1) {
    int i;
    if (!g) return -1;
    for(i=0; i < g->num_nodes; i++) {
        if (node_getId (g->nodes[i]) == nId1) return i;
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
    size = node_getConnect (g->nodes[index]);
    array = (int *) malloc(sizeof (int) * size);
    if (!array) {
        /* print error message */
        fprintf (stderr, "%s\n", strerror(errno));
        return NULL;
    }
    /* assign values to the array with the indices of the connected nodes*/
    for(i = 0; i< g->num_nodes; i++) {
        if (g->connections[index][i] == TRUE) {
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

    for (i = 0; i < MAX_NODES; i++){
        graphini->nodes[i] = NULL;
    }
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
    
    int i;

    if(g){
        for (i = 0; i < g->num_nodes; i++){
            node_free(g->nodes[i]);
        }
        free(g);
        g = NULL;
    }
    return;
}

Status graph_insertNode (Graph *g, const Node *n){
    
    int i, id;
    Node *cpy =NULL;

    if(!g || !n) return ERROR;

    id = node_getId(n);
    if (find_node_index(g, id) >= 0) return ERROR;
    
    cpy = node_copy(n);
    if (!cpy) return ERROR;

    g->nodes[g->num_nodes] = cpy;
    if(!g->nodes[g->num_nodes]) return ERROR; 

    if(!node_setNConnect(cpy, 0)) return ERROR;
    g->num_nodes++;
    for (i = 0; i < g->num_nodes; i++) {
        g->connections[g->num_nodes][i] = FALSE;
        g->connections[i][g->num_nodes] = FALSE;
    }

    return OK;
}

Status graph_insertEdge (Graph *g, const long nId1, const long nId2){

    int i1, i2, n;

    if (!g || nId1 < 0 || nId2 < 0) return ERROR;

    i1 = find_node_index(g, nId1);
    if (i1 == -1) return ERROR;
    i2 = find_node_index(g, nId2);
    if (i2 == -1) return ERROR;

    if(g->connections[i1][i2] == FALSE){
        g->connections[i1][i2] = TRUE;
        n = node_getConnect (g->nodes[i1]) + 1;
        node_setNConnect( g->nodes[i1], n);
        g->num_edges++;
        return OK;
    }

    return OK;
}

Node *graph_getNode (const Graph *g, long nId){

    Node *ret = NULL;
    int nIdx;

    if (!g || nId < 0) return NULL;

    nIdx = find_node_index(g, nId);
    if (nIdx == -1) return NULL;

    ret = node_copy(g->nodes[nIdx]);

    return ret;
}

Status graph_setNode (Graph *g, const Node *n){

    int nId;

    if (!g || !n) return ERROR;

    nId = find_node_index(g, node_getId(n));
    if (nId == -1) return ERROR;

    node_free(g->nodes[nId]);

    g->nodes[nId] = node_copy(n);

    return OK;
}

long * graph_getNodesId (const Graph *g){

    long *idNodes = NULL;
    int i;

    if (!g) return NULL;

    idNodes = (long*) malloc (g->num_nodes*sizeof(long));
    if (!idNodes) return NULL;

    for (i = 0; i < g->num_nodes; i++){
        idNodes[i] = node_getId(g->nodes[i]);
    }

    return idNodes;
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

    if (g->connections[idx1][idx2] == TRUE) return TRUE;

    return FALSE;
}

int graph_getNumberOfConnectionsFrom (const Graph *g, const long fromId){

    int nId;

    if(!g || fromId < 0) return -1;

    nId = find_node_index(g, fromId);
    if(nId == -1) return -1; 

    return node_getConnect(g->nodes[nId]);
}

long* graph_getConnectionsFrom (const Graph *g, const long fromId){

    int *nIndex = NULL, nIdx, i;
    Node *node = NULL;;

    if(!g || fromId < 0) return NULL;

    nIdx = find_node_index(g, fromId);
    if(nIdx == -1) return NULL;

   /* nIndex = (int*) malloc (sizeof(nIdx));
    if(!nIndex)return NULL; */
    
    nIndex = graph_getConnectionsIndex(g, nIdx);


    for(i = 0; i < node_getConnect(g->nodes[nIdx]); i++){
        node = g->nodes[nIndex[i]];    
        nIndex[i] = node_getId(node);    
    }

    free(nIdx);

    return (long*)nIndex;
}

int graph_print (FILE *pf, const Graph *g) {

    int connect, id, n = 0;
    long *graph = NULL, i, j;

    if (!pf || !g){
        fprintf (stderr, "%s\n", strerror(errno));
        return -1;
    }

    for (i = 0; i <= g->num_nodes; i++) {

        n = n + node_print(pf, g->nodes[i]);

        connect = node_getConnect(g->nodes[i]);
        id = node_getId(g->nodes[i]);
        
        graph = (long*) graph_getConnectionsFrom(g, id);
        if (!graph) return -1;

        for (j = 0; j < connect; j++) {
            n = n + fprintf(pf, "%li", graph[j]);
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
    Status aux2 = ERROR;
    Label label;
   
    if(fgets(aux, MAX_LINE, fin) != NULL) 
        if( sscanf(aux, "%d", &nnodes) != 1) return ERROR;
    
    n = node_init();
    if(!n) return ERROR;

    for(i=0; i < nnodes; i++) {
        if(fgets(aux, MAX_LINE, fin) != NULL) 
            if (sscanf(aux, "%d %s %li", &id1, name, &label) != NO_FILE_POS_VALUES) break;

        node_setId (n, id1);
        node_setName (n, name);
        node_setLabel(n, label);

        if(graph_insertNode (g, n) == ERROR) break;
    }
    
    if(i < nnodes) {
        node_free(n);
        return ERROR;
    }
    
    while(fgets(aux, MAX_LINE, fin)){
        if(sscanf(aux, "%d %d", &id1, &id2) == NO_FILE_POS_VALUES)
            if (graph_insertEdge(g, id1, id2) == ERROR) break;
    }
    
    if(feof(fin)) aux2 = OK;
    
    node_free(n);

    return aux2;
}