#include "node.h"

#define NAME_L 64 /*!< Maximum node name length */

struct _Node {
    char name[NAME_L]; /*!<Node name */
    long id; /*!<Node id */
    int nConnect; /*!<Node number of connections */
    Label label; /*!<Node state */
};

Node* node_init (){

    Node *nodeini = NULL;

    nodeini = (Node*)malloc(sizeof(Node));
    if (!nodeini){
        fprintf (stderr, "%s\n", strerror(errno));
        return NULL;
    }

    strcpy(nodeini->name, "\0");
    nodeini->id = -1;
    nodeini->nConnect = 0;
    nodeini->label = WHITE;
    
    return nodeini;
}

void node_free (void * n){
    if (n){
        free(n);
        n = NULL;
    }
    return;
}

long node_getId (const Node * n){

    if (!n) return -1;

    return n->id;
}

const char* node_getName (Node * n){

    if (!n) return NULL;

    return n->name;
}

int node_getConnect (const Node * n){

    if (!n) return -1;

    return n->nConnect;
}

Label node_getLabel (const Node*n){

    if (!n) return -1;

    return n->label;
}

Status node_setLabel (Node *n, Label l){

    if (!n || (l != WHITE && l != BLACK)) return ERROR;

    n->label = l;

    return OK;
}

Status node_setId (Node * n, const long id){

    if (!n || id < 0) return ERROR;

    n->id = id;

    return OK;
}

Status node_setName (Node *n, const char *name){

    if (!n || !name || strlen(name) >= NAME_L) return ERROR;

    strcpy(n->name, name);
    if (!n->name) return ERROR;

    return OK;
}

Status node_setNConnect (Node *n, const int cn){

    if (!n || cn < 0) return ERROR;

    n->nConnect = cn;

    return OK;
}


int node_cmp (const void *n1, const void *n2){

    Node *node1 = NULL, *node2 = NULL;

    int res = 0;
    
    if (!n1 || !n2) return -2;

    node1 = (Node*) n1;
    node2 = (Node*) n2;

    res = node1->id - node2->id;

    if (res == 0){
        res = strlen(node1->name) - strlen(node2->name);

        if (res == 0)return 0;
        if(res > 0) return 1;
        if (res < 0) return -1;
    }
    if (res > 0) return 1;
    if (res < 0) return -1;
    
    return -2;
}


void* node_copy (const void *src){
    
    Node *node = NULL, *trg = NULL;

    if (!src) return NULL;

    node = (Node*) src;

    trg = node_init();
    if (!trg) return NULL;

    trg->id = node->id;
    trg->nConnect = node->nConnect;
    strcpy(trg->name, node->name);
    trg->label = node->label;
    
    return trg;
}

/**
* @brief Prints in pf the data of a node
*
* The format will be: [id, name, label, nConnect]
* @code
* Node *n;
* n = node_init ();
* node_print (stdout, n);
* @endcode
* @param pf File descriptor
* @param n Node to be printted
* @return Returns the number of characters that have been written successfully.
* Checks if there have been errors in the output flow, in that case prints
* an error message in stderror and returns -1.
*/
int node_print (FILE *pf, const void *n){

    Node *node = NULL;

    if (!pf || !n){
        fprintf (stderr, "%s\n", strerror(errno));
        return -1;
    }

    node = (Node*) n;

    return fprintf(pf, "[%s, %li, %i, %i]", node->name, node->id, node->label, node->nConnect);
}