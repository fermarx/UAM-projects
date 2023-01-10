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

/***************************************************/
/* Function: InsertSort       Date: 04/10/2020     */
/* Your comment                                    */
/*                                                 */
/* Rutine that sorts a given list of numbers       */
/*                                                 */
/* Input:                                          */
/* int* table: Pointer to the list of numbers we   */
/* have to sort                                    */
/* int ip: First element of the table              */
/* int iu: Last element of the tablr               */
/* Output:                                         */
/* int : Times the OB has been executed            */
/***************************************************/
int InsertSort(int *table, int ip, int iu){

  int i = 0, j = 0, aux = 0, cont = 0;

  if (table == NULL || ip < 0 || iu < ip) return ERR;

  for (i = ip + 1; i <= iu; i++){
    aux = table[i];
    j = i - 1;
    while (j >= ip && table[j] > aux){
      table[j + 1] = table[j];
      j--;
      cont++;
    }
    
    if (j>=ip) cont++;
    table[j + 1] = aux;
  }

  return cont;
}

/***************************************************/
/* Function: InsertSortInv    Date: 04/10/2020     */
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
int InsertSortInv(int *table, int ip, int iu){

  int i = 0, j = 0, aux = 0, cont = 0;

  if (table == NULL || ip < 0 || iu < ip) return ERR;

  for (i = ip + 1; i <= iu; i++){
    aux = table[i];
    j = i - 1;
    while (j >= ip && table[j] < aux){
      table[j + 1] = table[j];
      j--;
      cont++;
    }
    if (j>=ip) cont++;
    table[j + 1] = aux;
  }
  return cont;
}