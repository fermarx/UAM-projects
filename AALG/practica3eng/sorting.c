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

/***************************************************/
/* Function: mergesort        Date: 01/11/2020     */
/*                                                 */
/* Rutine that sorts a given list of numbers by    */
/* dividing and combining                          */
/*                                                 */
/* Input:                                          */
/* int* table: Pointer to the list of numbers we   */
/* have to sort                                    */
/* int ip: First element of the table              */
/* int iu: Last element of the table               */
/* Output:                                         */
/* int : Times the OB has been executed            */
/***************************************************/
int mergesort(int* table, int ip, int iu){

  int middle = 0, aux = 0;
  int aux1 = 0, aux2 = 0, ob = 0;

  if (table == NULL || ip < 0 || ip > iu)return ERR;
  /*assert(table == NULL || ip < 0 || ip > iu);*/

  if (ip == iu)return OK;

  middle = (iu + ip) / 2;

  aux1 = mergesort(table, ip, middle);
  if (aux1 == ERR) return ERR;
  ob = ob + aux1;

  aux2 = mergesort(table, middle + 1, iu);
  if (aux2 == ERR) return ERR;
  ob = ob + aux2;

  aux = merge(table, ip, iu, middle);
  if (aux == ERR) return ERR;
  ob += aux;
  return ob;

}

/***************************************************/
/* Function: merge            Date: 01/11/2020     */
/*                                                 */
/* Rutine that sorts a given list of numbers by    */
/* dividing and combining                          */
/*                                                 */
/* Input:                                          */
/* int* table: Pointer to the list of numbers we   */
/* have to sort                                    */
/* int ip: First element of the table              */
/* int iu: Last element of the table               */
/* Output:                                         */
/* int : Times the OB has been executed            */
/***************************************************/
int merge(int* table, int ip, int iu, int imiddle){


  int i = ip, j = imiddle + 1, k = 0, cont = 0;
  int *auxtable = NULL;

  if (table == NULL || ip < 0 || iu < ip || imiddle > iu || imiddle < ip) return ERR;

  auxtable = (int *)calloc((iu - ip + 1), sizeof(auxtable[0]));
  if (auxtable == NULL) return ERR;

  while (i <= imiddle && j <= iu) {
    cont++;
    if (table[i] < table[j]){
      auxtable[k] = table[i];
      i++;
    }
    else{
      auxtable[k] = table[j];
      j++;
    }
    k++;
  }

  if (i > imiddle){
    while (j <= iu){
      auxtable[k] = table[j];
      j++;
      k++;
    }
  }
  else if (j > iu){
    while (i <= imiddle){
      auxtable[k] = table[i];
      i++;
      k++;
    }
  }

  for (i = ip, j = 0; i <= iu; i++, j++){
    table[i] = auxtable[j];
  }

  free(auxtable);

  return cont;

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

/***************************************************/
/* Function: quicksort_ntr  Date: 08/11/2020       */
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
int quicksort_ntr (int* table, int ip, int iu){
	int middle = 0, ob = 0, aux_left = 0;
	
	if (!table || ip < 0 || iu < 0 || iu < ip) return ERR;

	while (ip < iu) {

		ob += split(table, ip, iu, &middle);
		if (ob == ERR)return ERR;

		aux_left = quicksort_ntr(table, ip, middle);
		if (aux_left == ERR)return ERR;
		ob = ob + aux_left;

		ip = middle + 1;
	}
	return ob;
}