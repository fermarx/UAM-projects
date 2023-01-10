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
#define MAX_INT 2147483647
#define BILLION  1000000000
#define MAX_CHAR 100
#define CLOCK_REALTIME 0

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
  ptime->time = 0;

  if(method == NULL || n_perms<=0 || N<=0 || ptime == NULL) return ERR;
  
  table = generate_permutations (n_perms, N);
  if (table == NULL) return ERR;

  time1 = clock();
  if(time1 == (clock_t) -1){ 
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
  if(time2 == (clock_t) -1){
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

  ptime = calloc(num, sizeof(ptime[0])); 

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


/******************************************************************/
/* Function: generate_search_times        Date: 9/12/2020         */
/* Authors: S. Xiao Fdez                                          */
/*                                                                */
/* Function that generates search times from min to max using     */
/* increments of size incr                                        */
/*                                                                */
/* Input:                                                         */
/* pfunc_search method: pointer to a search function              */
/* pfunc_key_generator: pointer to a function that generates keys */
/* char order: indicates if the dictionary uses sorted tables     */
/* char* file: name of the file where the results are stored      */
/* int num_min: minimum size of permutations                      */
/* int num_max: maximum size of permutations                      */
/* int incr: increment in size                                    */
/* int n_times: number of times each key in the dictionary is     */
/* searched                                                       */
/*                                                                */
/* Output:                                                        */
/* returns ERR in case of error and OK in case the tables are     */
/* ordered correctly                                              */
/******************************************************************/
short generate_search_times(pfunc_search method, pfunc_key_generator generator, int order, char* file, int num_min, int num_max, int incr, int n_times){

  int perm = 0, i = 0, min = 0;
  short  control = 0;
  PTIME_AA ptime = NULL;

  if (method == NULL || generator == NULL || (order != SORTED && order!= NOT_SORTED) || file == NULL || num_max < num_min || incr <= 0 || n_times < 0) return ERR;

  perm = (((num_max - num_min) / incr) + 1);

  ptime = (PTIME_AA) calloc (perm, sizeof(ptime[0]));
  if (ptime == NULL) return ERR;

  min = num_min;

  for (i = 0; min <= num_max; i++, min +=incr) {
   control = average_search_time(method, generator, order, min, n_times, &ptime[i]);
    if (control == ERR) {
      free(ptime);
      ptime = NULL;
      return ERR;
    }
  }
  
  if (save_time_table (file, ptime, perm) == ERR) {
    free(ptime);
    ptime = NULL;
    return ERR;
  }

  free(ptime);
  ptime = NULL;

  return OK;
}


/******************************************************************/
/* Function: average_search_time        Date: 9/12/2020           */
/* Authors: S. Xiao Fdez                                          */
/*                                                                */
/* Function that calculates the average time the search algorithm */
/* takes                                                          */
/*                                                                */
/* Input:                                                         */
/* pfunc_search method: pointer to a search function              */
/* pfunc_key_generator: pointer to a function that generates keys */
/* char order: indicates if the dictionary uses sorted tables     */
/* int N: indicates the size of the dictionary                    */
/* int n_times: number of times each key in the dictionary is     */
/* searched                                                       */
/* PTIME_AA ptime: pointer to the structure TIME_AA               */
/*                                                                */
/* Output:                                                        */
/* returns ERR in case of error and OK in case the tables are     */
/* ordered correctly                                              */
/******************************************************************/
short average_search_time(pfunc_search metodo, pfunc_key_generator generator, int order, int N, int n_times, PTIME_AA ptime){

  PDICT dic = NULL;
  int ob = 0, insert = 0, time_ob = 0, i = 0, ppos = 0, aux;
  clock_t time1 = 0, time2 = 0;
  double times = 0;
  int *ele = NULL, *key = NULL;

  if (metodo == NULL || generator == NULL || (order != SORTED && order!= NOT_SORTED) || N < 1 || n_times < 1 || ptime == NULL) return ERR;

  ptime->average_ob = 0;
  ptime->max_ob = 0;
  ptime->min_ob = 0;
  ptime->N = 0;
  ptime->n_elems = 0;
  ptime->time = 0;

  dic = init_dictionary(N, order);
  if (dic == NULL) return ERR;

  ele = generate_perm(N);
  if (ele == NULL) {
    free_dictionary(dic);
    dic = NULL;
    return ERR;
  }

  insert = massive_insertion_dictionary(dic, ele, N);
  if (insert == ERR) {
    free(ele);
    ele = NULL;
    free_dictionary(dic);
    dic = NULL;
    return ERR;
  }

  aux = N*n_times;

  key = (int*) calloc(aux, (sizeof(key[0])));
  if (key == NULL){
    free(ele);
    free_dictionary(dic);
    return ERR;
  }

  generator(key, aux, N); /*void, so it does not return anything*/

  time1 = clock();
  if(time1 == (clock_t) -1){ 
    free(ele);
    free_dictionary(dic);
    return ERR;
  }

  for (i = 0; i < aux; i++){
    time_ob = search_dictionary(dic, key[i], &ppos, metodo);
    if (time_ob == ERR){
      free(key);
      free(ele);
      free_dictionary(dic);
      return ERR;
    }

    if (ptime->min_ob == 0 || ptime->min_ob > time_ob) ptime->min_ob = time_ob;
    if (ptime->max_ob == 0 || ptime->max_ob < time_ob) ptime->max_ob = time_ob;
    ob+=time_ob;
  }

  time2 = clock();
  if(time2 == (clock_t) -1){
    free(key);
    free(ele);
    free_dictionary(dic);
    return ERR;
  }
  times += ((time2-time1)/(double) CLOCKS_PER_SEC);
  times = times/aux;
  
  ptime->average_ob = (double) ob/aux;

  ptime->N = N;
  ptime->n_elems = n_times*N;
  ptime->time = times;

  free_dictionary(dic);
  free(key);
  free(ele);

  return OK;
}