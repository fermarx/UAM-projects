#include <stdlib.h>
#include <stdio.h>
#include "queue_fp.h"

#define MAXQUEUE 50

struct _Queue {
    void* items [MAXQUEUE];
    int front;
    int rear;
    free_element_function_type free_element_function;
    copy_element_function_type copy_element_function;
    print_element_function_type print_element_function;
};



/** PRIVATE FUNCTION */

/**
* @brief Returns whether the queue is full
* @param s A pointer to the queue.
* @return TRUE or FALSE
*/
Bool queue_isfull(const Queue *q);

Bool queue_isFull(const Queue *q){

    if (!q) return TRUE;

    if(q->front == (q->rear + 1) % MAXQUEUE) return TRUE;

    return FALSE;
}

                                    /** PUBLIC FUNCTIONS */

Queue* queue_init(free_element_function_type f1, copy_element_function_type f2, print_element_function_type f3){

    Queue *newQ = NULL;
    int i = 0;

    newQ = (Queue*)malloc(sizeof(Queue));
    if(!newQ) return NULL;

    newQ->front = 0;
    newQ->rear = 0;

    for (i=0; i < MAXQUEUE; i++)
        newQ->items[i] = NULL;

    newQ->free_element_function = f1;
    newQ->copy_element_function = f2;
    newQ->print_element_function = f3;

    return newQ;
}

void queue_free(Queue *q){
    int i;
    if (q== NULL) return;
    i = q->front;
    while (i != q->rear) {
        q->free_element_function(q->items[i]);
	    i = (i+1) % MAXQUEUE;
    }
    free(q);
}

Bool queue_isEmpty(const Queue *q){

    if (!q) return TRUE;

    if(q->front == q->rear) return TRUE;

    return FALSE;
}



Status queue_insert(Queue *q, const void* pElem){

    if (q == NULL || pElem == NULL || queue_isFull(q) == TRUE) return ERROR;

    q->items[q->rear] = q->copy_element_function(pElem);
    
    if (q->items[q->rear] == NULL) return ERROR;
    
    q->rear = (q->rear+1)%MAXQUEUE;
    return OK;
}

void * queue_extract(Queue *q){

    void *ret = NULL;

    if(!q || queue_isEmpty(q) == TRUE) return NULL;

    ret = q->items[q->front];
    if(!ret) return NULL;

    q->items[q->front] = NULL;

    q->front = (q->front + 1) % MAXQUEUE;

    return ret;
}

int queue_size (const Queue *q){
    
    if (!q) return -1;
    
    if (q->front < q->rear)
        return (q->rear - q->front);
    else 
        return ((q->front - q->rear));
}


int queue_print(FILE *pf, const Queue *q) {
    int count=0, i;
    
    if (!pf) return -1;

    if (queue_isEmpty(q)) return fprintf(pf, "Queue empty.\n");
    
    for (i=q->front; i!= q->rear; i=(i+1)%MAXQUEUE){
        count += q->print_element_function(pf, q->items[i]);
    }
    count += fprintf(pf, "\n");
    return count;
}