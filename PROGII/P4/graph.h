/**
* @file graph.h
* @author Profesores Prog2
* @date Created on 29 January 2020, 15:03
* @version 1.0
* @brief Library to manage TAD Graph
*
* @see
*/
#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"
#include "list.h"
#define MAXSTRING 65536

typedef struct _Graph Graph;

/**
* @brief Initializes a graph, reserving memory.
* @return Returns the graph address if it has done it correctly, otherwise
* it returns NULL and prints the string associated with error in stderror
**/
Graph * graph_init ();

/**
* @brief Frees the dynamic memory reserved for the graph
**/
void graph_free (Graph *g);

/**
* @brief Adds a node to the graph (reserving new memory for that node) only
* when there was no other node with the same id in the graph. It updates
* the necessary graph’s attributes.
* @param g Pointer to the graph where the new node to be inserted
* @param n Node to be inserted
* @return Returns OK or ERROR.
**/
Status graph_insertNode (Graph *g, const Node *n);

/**
* @brief Adds an edge from the node with id "nId1" to the node "nId2" in a
* directed graph.
* @param Pointer to the graph where the new edge to be inserted
* @param nId1, nId2 Nodes ids
* @return Returns OK or ERROR.
**/
Status graph_insertEdge (Graph *g, const long nId1, const long nId2);

/**
* @brief Returns a copy of the node of id "nId" storaged in the graph
* @param Pointer to the graph
* @param nId1 Node id
* @return Returns a ponter to the copy or NULL if the node does not exit
* in the graph.
**/
Node *graph_getNode (const Graph *g, long nId);

/**
* @brief Actualize the graph node with the same id
* @param g Pointer to the graph
* @param n
* @return Returns OK or ERROR.
**/
Status graph_setNode (Graph *g, const Node *n);

/**
* @brief Returns the address of an array with the ids of all nodes in the graph.
* Reserves memory for the array.
* @param g Pointer to the graph
* @return Returns the address of an array with the ids of all nodes in
* the graph or NULL if there are any error.
**/

long * graph_getNodesId (const Graph *g);

/**
* @brief Returns the total number of nodes in the graph.
* @param g Pointer to the graph
* @return Returns the number of nodes in the graph. -1 if there have been errors
**/
int graph_getNumberOfNodes (const Graph *g);

/**
* @brief Returns the total number of edges in the graph.
* @param g Pointer to the graph
* @return Returns the number of nodes in the graph. -1 if there have been
* errors
**/
int graph_getNumberOfEdges (const Graph *g);

/**
* @brief Determines if two nodes are connected.
* @param nId1, nId2 Nodes id
* @return Returns TRUE or FALSE
**/
Bool graph_areConnected (const Graph *g, const long nId1, const long nId2);

/**
* @brief Get the total number of connections from the node with a given id.
* @param fromId Node id
* @param g Graph
* @return Returns the total number of connections or -1 if there are any error
**/
int graph_getNumberOfConnectionsFrom (const Graph *g, const long fromId);

/**
* @brief Returns an array with the id of the nodes connected to a given node.
* Allocates memory for the array.
* @param g The graph
* @param fromId The id of the given node
* @return Returns the number of connections from the id node fromId or -1 if
* there are any error
*/
long* graph_getConnectionsFrom (const Graph *g, const long fromId);

/**
* @brief Prints in the file pf the data of a graph, returning the number of
* printed characters.
*
* The format to be followed is: print a line by node with the information
* associated with the node and the id of their connections:
* @code
* [1, a, 0, 2] 2 3
* [2, b, 0, 2] 1 3
* [3, c, 0, 2]] 1 2
* @endcode
* @param pf File descriptor
* @param g Pointer to the graph
* @return Return the number of characters printed.
* If there have been errors in the Output flow
* prints an error message in stderror and returns the value -1.
*/
int graph_print (FILE *pf, const Graph *g);

/**
* @brief Read from the stream pointer fin the graph information.
*
* This function is optative. See execirse 3.
* @param pf Stream pointer
* @param g Pointer to the graph
* @return OK or ERROR
*/
Status graph_readFromFile (FILE *fin, Graph *g);

/**
* @brief This function implements the DFS algorithm starting
* from the node with id ini_id
* @param pg, Graph
* @param ini_id, Origin node Id
* @param end_id, Destination node Id
12
* @param path, String with the traversed node's name.
* This parameter is modified by the function.
**/
Status graph_deepFirst (Graph *pg, long ini_id, long end_id, char *nodestraversed);

#endif /* GRAPH_H */