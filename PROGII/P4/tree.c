#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "tree.h"

typedef struct _BTNode {
    void *info;
    struct _BTNode *left;
    struct _BTNode *right;
} BTNode;

struct _BSTree {
    BTNode *root;
    destroy_element_function_type destroy_element;
    copy_element_function_type copy_element;
    print_element_function_type print_element;
    cmp_element_function_type cmp_element;
};

/*** DECLARACIÓN DE FUNCIONES PRIVADAS ***/

/* Create a BTNode*/
BTNode* bt_node_new();
/* Free a BTNode*/
void bt_node_free (BTNode *pn, destroy_element_function_type free_elem);
/*tree desctoy recursive*/
void tree_destroy_rec (BTNode* node, destroy_element_function_type des);
/*tree depth recursive*/
int tree_depth_rec(BTNode* node);
/*tree insert recursive*/
BTNode *tree_insert_rec(BSTree* tree, BTNode *node, const void *ele);
/*tree pre order recursive*/
void tree_preOrder_rec(FILE *f, BTNode *node, const BSTree *tree);
/*tree in order recursive*/
void tree_inOrder_rec(FILE *f, BTNode *node, const BSTree *tree);
/*tree post order recursive*/
void tree_postOrder_rec(FILE *f, BTNode *node, const BSTree *tree);


/*** PRIMITIVAS PRIVADAS DE BT-NODOS ***/
BTNode * bt_node_new(){
    BTNode* pn = NULL;
    pn = malloc(sizeof(BTNode));
    if (!pn) {
        fprintf (stderr,"%s\n", strerror (errno));
        return NULL;
    }
    pn->left = pn->right = NULL;
    pn->info = NULL;
    return pn;
}

void bt_node_free (BTNode *pn, destroy_element_function_type free_elem){
    if (!pn) return;
    free_elem(pn->info);
    free(pn);
}

/*** CREACIÓN Y LIBERACIÓN DE UN ÁRBOL ***/
/*Inicializa un árbol vacío.*/
BSTree* tree_init(destroy_element_function_type f1, copy_element_function_type f2, print_element_function_type f3, cmp_element_function_type f4){
    
    BSTree* tree = malloc (sizeof(BSTree));
    
    if (!tree) {
        fprintf (stderr,"%s\n", strerror (errno));
        return NULL;
    }
    
    tree->root = NULL;
    
    tree->destroy_element=f1;
    tree->copy_element=f2;
    tree->print_element=f3;
    tree->cmp_element=f4;
    
    return tree;
}

/*Indica si el árbol está o no vacio*/
Bool tree_isEmpty( const BSTree *tree){
	
    if (!tree || !tree->root || !tree->root->info) return TRUE;
	
    return FALSE;
}
/*Libera la memoria utilizada por un árbol.*/
void tree_destroy (BSTree *tree) {
	
    if (tree){
        tree_destroy_rec (tree->root, tree->destroy_element);
        free(tree);
        tree = NULL;
    }
	
    return;
}
void tree_destroy_rec (BTNode* node, destroy_element_function_type des){
	
    if (node){
	    tree_destroy_rec(node->left, des);
	    tree_destroy_rec(node->right, des);
	    bt_node_free(node, des);
    }
	
    return;
}

/*Indica la profundidad del árbo. Un árbol vacio profundidad -1*/
int tree_depth (const BSTree *tree){
	
    if (!tree || tree_isEmpty(tree) == TRUE) return -1;

	return tree_depth_rec (tree->root);
}
int tree_depth_rec(BTNode* node){
	
    int right, left;

	if (!node) return -1;

	if (node->right == NULL && node->left == NULL) return 0;

	right = 1 + tree_depth_rec(node->right);
	left = 1 + tree_depth_rec(node->left);
	
    if (right > left) return right;
	
    return left;
}

/*** INSERCIÓN ***/
/*Inserta un elemento en un árbol binario de búsqueda.*/
Status tree_insert (BSTree *tree, const void *elem){
    
    BTNode *new_ele = NULL;
    if (!tree||!elem) return ERROR;
    
    new_ele = tree_insert_rec(tree, tree->root, elem);

    if (!new_ele) return ERROR;
    
    tree->root = new_ele;
    return OK;
}

BTNode *tree_insert_rec(BSTree* tree, BTNode *node, const void *ele){
    
    int aux;
    BTNode *new_ele=NULL;

    if (!node){
        node = bt_node_new();
        if(!node) return NULL;
        node->info = tree->copy_element(ele);
        if(!node->info){
            bt_node_free(node, tree->destroy_element);
            return NULL;
        }
        return node;
    }
    aux = tree->cmp_element(ele, node->info);
    if (aux < 0){
        new_ele = tree_insert_rec(tree, node->left, ele);
        if (!new_ele)return NULL;
        node->left = new_ele;
        return node;
    }
    else if (aux > 0){
        new_ele = tree_insert_rec(tree, node->right, ele);
        if (!new_ele)return NULL;
        node->right = new_ele;
        return node;
    }
    return node;
}

/*** RECORRIDOS ***/
/*Recorre un árbol en orden previo.*/
void tree_preOrder (FILE *f, const BSTree *tree){
   
    if(!f || !tree || tree_isEmpty(tree)) return;

    tree_preOrder_rec(f, tree->root, tree);
    
    return;
}
void tree_preOrder_rec(FILE *f, BTNode *node, const BSTree *tree){
    
    if(!f || !node || !tree) return;

    tree->print_element(f, node->info);
    tree_preOrder_rec(f, node->left, tree);
    tree_preOrder_rec(f, node->right, tree);
}

/*Recorre un árbol en orden medio.*/
void tree_inOrder (FILE *f, const BSTree *tree){
   
    if (!f || !tree || tree_isEmpty(tree)) return;

    tree_inOrder_rec(f, tree->root, tree);
   
    return;
}
void tree_inOrder_rec(FILE *f, BTNode *node, const BSTree *tree){
    
    if(!f || !node || !tree) return;

    tree_inOrder_rec(f, node->left, tree);
    tree->print_element(f, node->info);
    tree_inOrder_rec(f, node->right, tree);
}

/*Recorre un árbol en orden posterior.*/
void tree_postOrder (FILE *f, const BSTree *tree){
    
    if (!f || !tree || tree_isEmpty(tree)) return;

    tree_postOrder_rec(f, tree->root, tree);
    
    return;
}
void tree_postOrder_rec(FILE *f, BTNode *node, const BSTree *tree){
    
    if(!f || !node || !tree) return;

    tree_postOrder_rec(f, node->left, tree);
    tree_postOrder_rec(f, node->right, tree);
    tree->print_element(f, node->info);
}