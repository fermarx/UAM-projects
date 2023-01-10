/**
 *
 * Descripcion: Header file for sorting functions 
 *
 * Fichero: sorting.h
 * Autor: Carlos Aguirre
 * Version: 1.0
 * Fecha: 16-09-2019
 *
 */

#ifndef ORDENACION_H
#define ORDENACION_H
#include <stdlib.h>
#include <stdio.h>

/* constants */

#ifndef ERR
  #define ERR -1
  #define OK (!(ERR))
#endif

/* type definitions */
typedef int (* pfunc_sort)(int*, int, int);

/* helping functions */
int Swap(int *first, int *second);

/* Functions */
int InsertSort(int* table, int ip, int iu);
int InsertSortInv(int* table, int ip, int iu);

int mergesort(int* table, int ip, int iu);
int merge(int* table, int ip, int iu, int imiddle);

int quicksort(int* table, int ip, int iu);
int split(int* table, int ip, int iu,int *pos);
int median(int *table, int ip, int iu,int *pos);
int quicksort_ntr(int* table, int ip, int iu);

#endif
