/**
 *
 * Descripcion: Implementation of sorting functions
 *
 * Fichero: sorting.c
 * Autor: Carlos Aguirre
 * Version: 1.0
 * Fecha: 16-09-2019
 *
 */


#include "sorting.h"
#include <assert.h>

/****************************************/
/* Function: Swap     Date: 09/12/2020  */
/*                                      */
/* Swaps two given numbers              */
/*                                      */
/* Input:                               */
/* int *first: pointer to the number we */
/* want to swap                         */
/* int *second: pointer to the number we*/
/* want to swap                         */
/****************************************/
int Swap(int *first, int *second) {

  int aux = 0;

  if(*first < 0 || *second < 0) return ERR;

  aux = *first;
  *first = *second;
  *second = aux;

  return OK;
}

/***************************************************/
/* Function: quicksort    Date: 08/11/2020         */
/* Authors: S. Xiao Fernandez                      */
/*                                                 */
/* Rutine that sorts a given list of numbers by    */
/* dividing and combining                          */
/*                                                 */
/*                                                 */
/* Input:                                          */
/* int* table: Pointer to the list of numbers we   */
/* have to sort                                    */
/* int ip: First element of the table              */
/* int iu: Last element of the tablr               */
/* Output:                                         */
/* int : Times the OB has been executed            */
/***************************************************/
int quicksort(int* table, int ip, int iu){

  int middle = 0, ob = 0, aux_left = 0, aux_right = 0;

  if (table == NULL || ip < 0 || iu < 0 || iu < ip) return ERR;

  if (ip == iu) return OK;

  ob += split(table, ip, iu, &middle);
  if (ob == ERR)return ERR;

  if (ip < middle - 1) {
    aux_left = quicksort(table, ip, middle - 1);
    if (aux_left == ERR)return ERR;
    ob = ob + aux_left;
  }

  if (middle + 1 < iu) {
    aux_right = quicksort(table, middle + 1, iu);
    if (aux_right == ERR) return ERR;
    ob = ob + aux_right;
  }

  return ob;

}

/***************************************************/
/* Function: split    Date: 08/11/2020             */
/* Authors: S. Xiao Fernandez                      */
/*                                                 */
/* Rutine that divides a given list of numbers     */
/*                                                 */
/*                                                 */
/* Input:                                          */
/* int* table: Pointer to the list of numbers we   */
/* have to sort                                    */
/* int ip: First element of the table              */
/* int iu: Last element of the table               */
/* Output:                                         */
/* int : Times the OB has been executed            */
/***************************************************/
int split(int* table, int ip, int iu,int *pos){

  int i = 0, k = 0, ob = 0;

  if (table == NULL || ip < 0 || iu < 0 || iu < ip || pos == NULL) return ERR;

  ob = median(table, ip, iu, pos);
  if (ob == ERR) return ERR;

  if ((*pos) > iu || (*pos) < ip) return ERR;

  k = table[*pos];

  if(Swap(&table[ip], &table[*pos]) == ERR) return ERR;

  *pos = ip;

  for (i = ip + 1; i <= iu; i++){
    if (table[i] < k){
      (*pos)++;
      if(Swap(&table[i], &table[*pos]) == ERR) return ERR;
    }
    ob++;
  }

  if(Swap(&table[ip], &table[*pos]) == ERR) return ERR;

  return ob;

}

/***************************************************/
/* Function: median    Date: 08/11/2020            */
/* Authors: S. Xiao Fernandez                      */
/*                                                 */
/* Rutine that sorts a given list of numbers in    */
/* inverse order                                   */
/*                                                 */
/* Input:                                          */
/* int* table: Pointer to the list of numbers we   */
/* have to sort                                    */
/* int ip: First element of the table              */
/* int iu: Last element of the table               */
/* Output:                                         */
/* int : Times the OB has been executed            */
/***************************************************/
int median(int *table, int ip, int iu,int *pos){

  if (table == NULL || ip < 0 || iu < 0 || iu < ip || pos == NULL) return ERR;

  *pos = ip;

  return OK;

}
