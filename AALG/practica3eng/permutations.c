/**
 *
 * Descripcion: Implementation of function that generate permutations
 *
 * File: permutations.c
 * Autor: Carlos Aguirre
 * Version: 1.0
 * Fecha: 21-09-2019
 *
 */


#include "permutations.h"
#include "times.h"

/***************************************************/
/* Function: random_num Date: 04/10/2020           */
/* Authors: S. Xiao Fernandez Marin                */
/*                                                 */
/* Rutine that generates a random number           */
/* between two given numbers                       */
/*                                                 */
/* Input:                                          */
/* int inf: lower limit                            */
/* int sup: upper limit                            */
/* Output:                                         */
/* int: random number                              */
/***************************************************/
int random_num(int inf, int sup){

  int random = 0;  

  if (sup < inf|| inf < 0|| sup > RAND_MAX) return ERR;

  random = (int) inf + rand() / ((RAND_MAX + 1.0) / (sup - inf + 1)); 
  
  return random;
}


/***************************************************/
/* Function: generate_perm Date: 04/10/2020        */
/* Authors: S. Xiao Fernandez Marin                */
/*                                                 */
/* Rutine that generates a random permutation      */
/*                                                 */
/* Input:                                          */
/* int n: number of elements in the permutation    */
/* Output:                                         */
/* int *: pointer to integer array                 */
/* that contains the permitation                   */
/* or NULL in case of error                        */
/***************************************************/
int* generate_perm(int N){

  int i = 0, num1 = 0, num2 = 0;
  int *perm = NULL;

  if (N <= 0) return NULL;

  perm = (int*) calloc ( N, sizeof(perm[0]));
  if (perm == NULL) return NULL;

  for (i = 0; i < N; i++){
    perm[i] = i + 1;
  }

  for (i = 0; i < N; i++){
    num1 = perm[i];
    num2 = random_num(i, N-1);
    perm[i] = perm[num2];
    perm[num2] = num1;
  }

  return perm;
}


/***************************************************/
/* Function: generate_permutations Date: 04/10/2020*/
/* Authors: S. Xiao Fernandez Marin                */
/*                                                 */
/* Function that generates n_perms random          */
/* permutations with N elements                    */
/*                                                 */
/* Input:                                          */
/* int n_perms: Number of permutations             */
/* int N: Number of elements in each permutation   */
/* Output:                                         */
/* int**: Array of pointers to integer that point  */
/* to each of the permutations                     */
/* NULL en case of error                           */
/***************************************************/
int** generate_permutations(int n_perms, int N) {

  int **perm = NULL;
  int i=0;

  if (n_perms <= 0 || N <= 0) return NULL;
  
  perm = (int**) calloc ((n_perms), sizeof(perm[0]));
  if (perm == NULL) return NULL;

  for (i = 0; i < n_perms; i++){
      perm [i]= generate_perm(N);
      
      if (perm[i] == NULL) {
          for (i = i-1; i >= 0; i--) {
            free (perm[i]);
          }
          free(perm);
          return NULL;
      }

  }

  return perm;  
}