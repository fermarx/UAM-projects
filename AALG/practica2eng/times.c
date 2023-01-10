/**
 *
 * Descripcion: Implementation of time measurement functions
 *
 * Fichero: times.c
 * Autor: Carlos Aguirre Maeso
 * Version: 1.0
 * Fecha: 16-09-2019
 *
 */

#include <limits.h>
#include "times.h"

/******************************************************************/
/* Function: average_sorting_time Date: 04/10/2020                */
/* Authors: S. Xiao Fernandez Marin                               */
/*                                                                */
/* Function that calculates the average time the sorting algorithm*/
/* takes                                                          */
/*                                                                */
/* Input:                                                         */
/* pfunc_sort: Pointer to the sort function, defined as:          */
/* typedef int (* pfunc sort) (int *, int, int);                  */
/* int n_perms: Represents the number of perms. to be generated   */
/* and sorted by the used method (in this case insertion method)  */
/* int N: Size of each permutation                                */
/* ptime: Pointer to the type structure TIME AA                   */
/*                                                                */
/* Output:                                                        */
/* returns ERR in case of error and OK in case the tables are     */
/* ordered correctly                                              */
/******************************************************************/
short average_sorting_time(pfunc_sort method, int n_perms, int N, PTIME_AA ptime){

  clock_t time1 = 0, time2 = 0; 
  int i = 0, j = 0, time_ob = 0, ob=0;
  int **table = NULL;
  double times = 0;

  ptime->average_ob = 0;
  ptime->max_ob = 0;
  ptime->min_ob = INT_MAX;
  ptime->N = 0;
  ptime->n_elems = 0;
  ptime->time = 0;

  if(method == NULL || n_perms<=0 || N<=0 || ptime == NULL) return ERR;
  
  table = generate_permutations (n_perms, N);
  if (table == NULL) return ERR;

  time1 = clock();
  if(time1 == -1){
    free(table);
    return ERR;
  }
  for (i = 0; i < n_perms; i ++){
    time_ob = method(table[i], 0, N - 1);
    if (time_ob == ERR){
        for(j = i; j >= 0; j--) {
          free(table[j]);
          return ERR;
        }
        free(table);
    }
    if (ptime->min_ob == 0 || ptime->min_ob > time_ob) ptime->min_ob = time_ob;
    if (ptime->max_ob == 0 || ptime->max_ob < time_ob) ptime->max_ob = time_ob;
    ob+=time_ob;
  }
  
  time2 = clock();
  if(time2 == -1){
    for(j = i; (j = 0); j--) {
      free(table[j]);
      return ERR;
    }
    free(table);
    return ERR;
  }
  times += ((time2-time1)/(double) CLOCKS_PER_SEC);
  times = times/(double)n_perms;
  
  ptime->average_ob = (double) ob/n_perms;

  ptime->N = N;
  ptime->n_elems = n_perms;
  ptime->time = times;

  for(i=0;i<n_perms;i++) free(table[i]);
  free(table);

  return OK;
}


/******************************************************************/
/* Function: generate_sorting_times Date: 04/10/2020              */
/* Authors: S. Xiao Fernandez Marin                               */
/*                                                                */
/* Function that calculates the average time the sorting algorithm*/
/* takes                                                          */
/*                                                                */
/* Input:                                                         */
/* pfunc_sort: Pointer to the sort function, defined as:          */
/* typedef int (* pfunc sort) (int *, int, int);                  */
/* char* file: File where the average clock time and the average, */
/* minimum and maximum number of times that the OB is executed    */
/* is written                                                     */
/* int num_min: Minimum size of permutations                      */
/* int num_max: Maximum size of permutations                      */
/* int incr: Increment in size                                    */
/* int n_perms: Number of permutations                            */
/*                                                                */
/* Output:                                                        */
/* returns ERR in case of error and OK in case the tables are     */
/* ordered correctly                                              */
/******************************************************************/
short generate_sorting_times(pfunc_sort method, char* file, int num_min, int num_max, int incr, int n_perms){

  int i = 0, num = 0, counter = 0, control = 0;
  PTIME_AA ptime = NULL;

  if (method == NULL || file == NULL || num_min <0 || num_min > num_max || incr <= 0 || n_perms <= 0) return ERR;


  /*Number of permutations*/
  num = (num_max - num_min) / incr + 1;

  ptime = malloc(num * sizeof(ptime[0])); 

  if (ptime == NULL) return ERR;

  /*Generates permutations and shave them in the corresponponding structures */
  for (i = num_min; i<= num_max; i = i + incr, counter++) {
    control = average_sorting_time(method, n_perms, i, &ptime[counter]);
      if (control == ERR) {
        free(ptime);
        return ERR;
      }
  }
  /*Call the save_time_table function to print all data obtained*/

  if (save_time_table (file, ptime, num) == ERR) {
    free(ptime);
    return ERR;
  }

  free(ptime);

  return OK;
 
}


/******************************************************************/
/* Function: save_time_table Date: 04/10/2020                     */
/* Authors: S. Xiao Fernandez Marin                               */
/*                                                                */
/* Function that calculates the average time the sorting algorithm*/
/* takes                                                          */
/*                                                                */
/* Input:                                                         */
/* char* file: File where the average clock time and the average, */
/* minimum and maximum number of times that the OB is executed    */
/* is written                                                     */
/* ptime: Pointer to the type structure TIME AA                   */
/* int n_times: Number of array elements                          */
/*                                                                */
/* Output:                                                        */
/* returns ERR in case of error and OK in case the tables are     */
/* ordered correctly                                              */
/******************************************************************/
short save_time_table(char* file, PTIME_AA ptime, int n_times){

  int i=0;
  FILE *f = NULL;

  if(file == NULL || ptime == NULL || n_times < 0) return ERR;
  
  f = fopen (file, "w");
  
  if (f== NULL) return ERR;

  if(fprintf(f, "N\t") < 0) return ERR;
  if(fprintf(f, "time\t") < 0) return ERR;
  if(fprintf(f, "average ob\t") < 0) return ERR;
  if(fprintf(f, "max ob\t") < 0) return ERR;
  if(fprintf(f, "min ob\t") < 0) return ERR;
  if(fprintf(f, "\n") < 0) return ERR;

  for (i=0; i< n_times; i++) {
    if(fprintf(f, "%d\t", ptime[i].N) < 0) return ERR;
    if(fprintf(f, "%f\t", ptime[i].time) < 0) return ERR;
    if(fprintf(f, "%f\t",ptime[i].average_ob) < 0) return ERR;
    if(fprintf(f, "%d\t", ptime[i].max_ob) < 0) return ERR;
    if(fprintf(f, "%d\t",ptime[i].min_ob) < 0) return ERR;
    if(fprintf(f, "\n") < 0) return ERR;
  }
    
  if(fclose(f)!=0) return ERR;

  return OK;

}