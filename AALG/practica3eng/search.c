/**
 *
 * Description: Implementation of functions for search
 *
 * File: search.c
 * Author: Carlos Aguirre and Javier Sanz-Cruzado
 * Version: 1.0
 * Date: 14-11-2016
 *
 */

#include "search.h"
#include "sorting.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

/**
 *  Key generation functions
 *
 *  Description: Receives the number of keys to generate in the n_keys
 *               parameter. The generated keys go from 1 to max. The
 * 				 keys are returned in the keys parameter which must be 
 *				 allocated externally to the function.
 */
  
/**
 *  Function: uniform_key_generator
 *               This function generates all keys from 1 to max in a sequential
 *               manner. If n_keys == max, each key will just be generated once.
 */
void uniform_key_generator(int *keys, int n_keys, int max)
{
  int i;

  for(i = 0; i < n_keys; i++) keys[i] = 1 + (i % max);

  return;
}

/**
 *  Function: potential_key_generator
 *               This function generates keys following an approximately
 *               potential distribution. The smaller values are much more 
 *               likely than the bigger ones. Value 1 has a 50%
 *               probability, value 2 a 17%, value 3 the 9%, etc.
 */
void potential_key_generator(int *keys, int n_keys, int max)
{
  int i;

  for(i = 0; i < n_keys; i++) 
  {
    keys[i] = .5+max/(1 + max*((double)rand()/(RAND_MAX)));
  }

  return;
}


/***************************************************/
/* Function: init_dictionary    Date: 28/11/2020   */
/* Authors: S. Xiao Fdez                           */
/*                                                 */
/* Function that creates an empty dictionary of the*/
/* type indicated by its parameters.               */
/*                                                 */
/* Input:                                          */
/* int size: initial size we want the dictionary   */
/* to have                                         */
/* char order: indicates if the list is SORTED or  */
/* NOT_SORTED                                      */
/*                                                 */
/* Output:                                         */
/* PDICT: the new initialised dictionary           */
/***************************************************/
PDICT init_dictionary (int size, char order){
	
  PDICT new_dictionary = NULL;
  
  if (size < 0 || (order != NOT_SORTED && order != SORTED)) return NULL;

  new_dictionary = (PDICT) calloc (1,sizeof(new_dictionary[0]));
  if (new_dictionary == NULL) return NULL;
  
  new_dictionary->table = NULL;
  new_dictionary->table = (int*)calloc(size, sizeof(new_dictionary->table[0]));
  if (new_dictionary->table == NULL) {
    free_dictionary(new_dictionary);
    return NULL;
  }

  new_dictionary->size = size;
  new_dictionary->n_data = 0;
  new_dictionary->order = order;

  return new_dictionary;
}


/***************************************************/
/* Function: free_dictionary     Date: 28/11/2020  */
/* Authors: S. Xiao Fdez                           */
/*                                                 */
/* Function that frees all the memory allocated of */ 
/* the dictionary                                  */
/*                                                 */
/* Input:                                          */
/* PDICT pdict: pointer to the struct dictionary   */
/***************************************************/
void free_dictionary(PDICT pdict){
  if (pdict == NULL) return;
  if (pdict->table){
    free(pdict->table);
    pdict->table = NULL;
  }
  free(pdict);
  pdict = NULL;
}


/***************************************************/
/* Function: insert_dictionary   Date: 28/11/2020  */
/* Authors: S. Xiao Fdez                           */
/*                                                 */
/* Function that introduces the key element in the */
/* adequate position of the dictionary             */
/*                                                 */
/* Input:                                          */
/* PDICT pdict: pointer to the struct of the       */
/* dictionary                                      */
/* int key: the element we want to search for in   */
/* the dictionary                                  */
/*                                                 */
/* Output:                                         */
/* int: Returns the number of basic operations     */
/* if everything work correctly and if not,        */
/* it returns ERROR                                */
/***************************************************/
int insert_dictionary(PDICT pdict, int key){
	
  int i = 0, ob = 0, A = 0;

  if (pdict == NULL || key < 0 || pdict->n_data < 0|| pdict->n_data >= pdict->size) return ERR;

  if(pdict->order == NOT_SORTED){
    pdict->table[pdict->n_data] = key;
    pdict->n_data++;

  }else if (pdict->order == SORTED){
    pdict->table[pdict->n_data] = key;

    A = pdict->table[pdict->n_data];
    i = pdict->n_data - 1;
    
    while (i >= 0 && pdict->table[i] > A){
      pdict->table[i + 1] = pdict->table[i];
      ob++;
      i--;
    }
    if (i >= 0) ob++;
    pdict->table[i + 1] = A;
    pdict->n_data++;
  }
  return ob;
}


/************************************************************/
/* Function: massive_insertion_dictionary  Date: 28/11/2020 */
/* Authors: S. Xiao Fdez                                    */
/*                                                          */
/* Function that introduces the keys in the keys table in   */
/* the dictionary                                           */
/*                                                          */
/* Input:                                                   */
/* PDICT pdict: pointer to the struct of the dictionary     */
/* int *keys: keys to be inserted                           */
/* int n_keys: number of keys to be inserted                */
/*                                                          */
/* Output:                                                  */
/* int: Returns the number of BO's if everything worked     */
/* correctly or in the contrary, ERROR                      */
/************************************************************/
int massive_insertion_dictionary (PDICT pdict,int *keys, int n_keys){

  int i = 0, ob = 0, aux = 0;

  if (pdict == NULL || keys == NULL || n_keys < 0) return ERR;
  
  for (i = 0; i < n_keys; i++){
    aux = insert_dictionary(pdict, keys[i]);
    if (aux == ERR){
      return ERR;
    }
    ob += aux;
  }
  return ob;
}


/***************************************************/
/* Function: search_dictionary    Date: 28/11/2020 */
/* Authors: S. Xiao Fdez                           */
/*                                                 */
/* Function that looks for a key in the dictionary */
/*                                                 */
/* Input:                                          */
/* PDICT pdict: pointer to the struct of the       */
/* dictionary                                      */
/* int key: key we want to look for                */
/* int *ppos: memory position where the key is     */
/* stored                                          */
/* pfunc_search method: pointer to a search        */
/* function                                        */
/*                                                 */
/* Output:                                         */
/* int: Returns the number of BO's if everything   */
/* worked correct, or in the contrary, ERROR       */
/***************************************************/
int search_dictionary(PDICT pdict, int key, int *ppos, pfunc_search method){
	
  int ob = 0;

  if (pdict == NULL || key < 0 || ppos == NULL || method == NULL) return ERR;

  ob = method(pdict->table, 0, (pdict->n_data) - 1, key, ppos);
  if (ob == ERR) return ERR;

  return ob;
}


/* Search functions of the Dictionary ADT */

/***************************************************/
/* Function: bin_search           Date: 28/11/2020 */
/* Authors: S. Xiao Fdez                           */
/*                                                 */
/* Function that looks for a key in the dictionary */
/* using the binary search algorithm               */
/*                                                 */
/* Input:                                          */
/* int *table: pointer to the data table           */
/* dictionary                                      */
/* int F: first element of the table               */
/* int L: last element  of the table               */
/* int key: key we want to look for                */
/* int *ppos: memory position where the key is     */
/* stored                                          */
/*                                                 */
/* Output:                                         */
/* int: Returns the number of BO's if everything   */
/* worked correct, NOT_FOUND if the key is not in  */
/* the table                                       */
/***************************************************/
int bin_search(int *table,int F,int L,int key, int *ppos){
  
  int ob = 0, mid=0, start = F, end = L, result = 0;

  if (table == NULL || F > L || F < 0 || L < 0 || key < 0 || ppos == NULL) return ERR;

  while (start <= end){
    mid = (end + start) / 2;
    ob++;

    result = table[mid] - key;

    if(result == 0){
      *ppos = mid;
      return ob;
    }
    
    if (result < 0){
      start = mid + 1;
    }else{
      end = mid - 1;
    }
  }
  *ppos=NOT_FOUND;
  return NOT_FOUND;
}

/***************************************************/
/* Function: lin_search   Date: 28/11/2020         */
/* Authors: S. Xiao Fdez                           */
/*                                                 */
/* Function that looks for a key in the dictionary */
/* using the linear search algorithm               */
/*                                                 */
/* Input:                                          */
/* int *table: pointer to the data table           */
/* dictionary                                      */
/* int F: first element of the table               */
/* int L: last element  of the table               */
/* int key: key we want to look for                */
/* int *ppos: memory position where the key is     */
/* stored                                          */
/*                                                 */
/* Output:                                         */
/* int: Returns the number of BO's if everything   */
/* worked correct, NOT_FOUND if the key is not in  */
/* the table                                       */
/***************************************************/
int lin_search(int *table,int F,int L,int key, int *ppos){
	
  int i = 0, ob = 0;

  if (table == NULL || F > L || F < 0 || L < 0 || key < 0 || ppos == NULL) return ERR;

  for (i = F; i <= L; i++){
    ob++;
    if (table[i] == key){
      *ppos = i;
      return ob;
    }
  }

  *ppos=NOT_FOUND;
  return NOT_FOUND;
}


/***************************************************/
/* Function: lin_auto_search   Date: 28/11/2020    */
/* Authors: S. Xiao Fdez                           */
/*                                                 */
/* Function that looks for a key in the dictionary */
/* using the auto-organized linear search algorithm*/
/*                                                 */
/* Input:                                          */
/* int *table: pointer to the data table           */
/* dictionary                                      */
/* int F: first element of the table               */
/* int L: last element  of the table               */
/* int key: key we want to look for                */
/* int *ppos: memory position where the key is     */
/* stored                                          */
/*                                                 */
/* Output:                                         */
/* int: Returns the number of BO's if everything   */
/* worked correct, NOT_FOUND if the key is not in  */
/* the table                                       */
/***************************************************/
int lin_auto_search(int *table,int F,int L,int key, int *ppos){

  int i = 0, ob = 0;

  if (table == NULL || F > L || F < 0 || L < 0 || key < 0 || ppos == NULL) return ERR;
  
  ob++;

  if (table[F] == key){
    *ppos = F;
    return ob;
  }

  for (i = F+1; i <= L; i++){
    ob++;
    if (table[i] == key){
      if(i > 0){
        *ppos = i - 1;
        if(Swap(&table[i], &table[i-1]) == ERR) return ERR;
        return ob;
      }
    }
  }

  *ppos=NOT_FOUND;
  return NOT_FOUND;
}

