#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "index.h"

typedef struct rec_{
  long *rec;
  int n_rec;
  int key;
}rec_t;

struct index_ {
  FILE *newfile;
  type_t type;
  rec_t **recs;
  int n_recs;
  int i_pos;
  int f_pos;
};

int binary(rec_t**, int, int);

int binary(rec_t **table, int L, int key){
	
  int mid=0, F = 0;

  if (!table || L < 0 || key < 0) return F + 1;

 while (F <= L){
   mid = (L + F) / 2;
   if (table[mid]->key == key){
     return mid;
   }else if (table[mid]->key < key){
     F = mid + 1;
     if (F > L) return -(F + 1);
   }else{
     L = mid -1;
   }
 }
  return -(mid + 1);
}

int index_create(char *path, type_t type) {
 
  FILE *newfile = NULL;
  int write = 0, num = 0;

  if (!path || type < 0) return 0;

  newfile = fopen(path, "r+");
  if (!newfile) return 0;

  write = fwrite(&type, sizeof(type_t), 1, newfile); 
  if (write == -1){
    fclose(newfile);
    return 0;
  }
  
  /* We write in the file a 0 because it is empty */
  write = fwrite(&num, sizeof(int), 1, newfile); 
  if (write == -1){
    fclose(newfile);
    return 0;
  }

  fclose(newfile);

  return 1;
}

index_t* index_open(char* path) {

  index_t *new_index = NULL;
  int read = 0, i = 0, j = 0, pos = 0, key = 0, len = 0;
  long *aux = NULL;
  rec_t *aux_rec = NULL;

  if (!path) return NULL;

  new_index = (index_t*)malloc(sizeof(index_t));
  if (!new_index) return NULL;

  new_index->newfile = fopen(path, "r+");
  if (!new_index->newfile){
    free(new_index);
    return NULL;
  }

  fseek(new_index->newfile, 0, SEEK_SET);
  if(fread(&(new_index->type), sizeof(int), 1, new_index->newfile) == 0){
    fclose(new_index->newfile);
    free(new_index);
    return NULL;
  }

  new_index->i_pos = ftell(new_index->newfile);

  read = fread(&new_index->n_recs, sizeof(int), 1, new_index->newfile); 
  if (read == -1){
    fclose(new_index->newfile);
    free (new_index);
    return NULL;
  }

  new_index->f_pos = ftell(new_index->newfile);

  new_index->recs = (rec_t**)calloc(1, sizeof(rec_t*));
  if (!new_index->recs){
    fclose(new_index->newfile);
    free (new_index);
    return NULL;
  }

  if(new_index->n_recs != 0){

  pos = new_index->f_pos;
  for(i=0; i < new_index->n_recs; i++){

    fseek(new_index->newfile, pos, SEEK_SET);

    if(fread(&key, sizeof(int), 1, new_index->newfile) == 0){
      fclose(new_index->newfile);
      free(new_index);
      return NULL;
    }
    
    if(fread(&len, sizeof(int), 1, new_index->newfile) == 0){
      fclose(new_index->newfile);
      free(new_index);
      return NULL;
    }
      
    aux = (long *) calloc(len, sizeof(long));
    if(!aux){
      fclose(new_index->newfile);
      free(new_index);
      return NULL;
    }

    if(fread(aux, sizeof(long), len, new_index->newfile) == 0){
      fclose(new_index->newfile);
      free(new_index);
      free(aux);
      return NULL;
    }

    for(j = 0; j < len; j++){
      if(index_put(new_index, key, aux[i]) == 0){
        fclose(new_index->newfile);
        free(new_index);
        free(aux);
        return NULL;
      }
    }

    free(aux);
    pos = ftell(new_index->newfile);
    }
  }

  return new_index;
}

int index_save(index_t* idx) {

  int i = 0, write = 0;

  if (idx == 0) return 0;

  fseek(idx->newfile, 0, SEEK_SET);

  write = fwrite(&idx->type, sizeof(type_t), 1, idx->newfile);
  if(write == 0) return 0;    

  write = fwrite(&idx->n_recs, sizeof(int), 1, idx->newfile);
  if(write == 0) return 0;

  for (i = 0; i < idx->n_recs; i++){

    write = fwrite(&idx->recs[i]->key, sizeof(int), 1, idx->newfile);
    if(write == 0) return 0;

    write = fwrite(&idx->recs[i]->n_rec, sizeof(int), 1, idx->newfile);
    if(write == 0) return 0;
    
    write = fwrite(&idx->recs[i]->rec, sizeof(long), idx->recs[i]->n_rec, idx->newfile);
    if(write == 0) return 0;
  }

  return 1;
}

int index_put(index_t *idx, int key, long pos) {

  int i = 0, k = 0;
  rec_t *recaux = NULL;
  long *recaux2 = NULL;

  if (!idx || pos < 0) return 0;

  if (idx->n_recs == 0){
    idx->n_recs++;

    recaux2 = (long*) calloc (1, sizeof(long));
    if (!recaux2) return 0;

    recaux = (rec_t*) calloc(1, sizeof(rec_t));
    if (!recaux){
      free(recaux2);
      return 0;
    }

  recaux->key = key;
  recaux->n_rec = 1;
  recaux2[recaux->n_rec - 1] = pos;

  recaux->rec = recaux2;
  recaux2 = NULL;

  idx->recs[idx->n_recs - 1] = recaux;
  recaux = NULL;

  return idx->n_recs;    
  }

  k = binary (idx->recs, idx->n_recs - 1, key);
  if (k == idx->n_recs) return 0;
  else if (k >= 0){

    (idx->recs[k]->n_rec)++;

    idx->recs[k]->rec = realloc(idx->recs[k]->rec, idx->recs[k]->n_rec*sizeof(long));
    if (!idx->recs[k]->rec) return 0;

    idx->recs[k]->rec[(idx->recs[k]->n_rec) - 1] = pos;
    
  }else{

    k = -(k + 1);
    idx->n_recs++;

    idx->recs = realloc(idx->recs, idx->n_recs*sizeof(rec_t));
    if (!idx->recs) return 0;

    recaux2 = (long*) calloc (1, sizeof(long));
    if (!recaux2) return 0;

    recaux = (rec_t*) calloc(1, sizeof(rec_t));
    if (!recaux){
      free(recaux2);
      return 0;
    } 

    recaux->key = key;
    recaux->n_rec = 1;
    recaux2[recaux->n_rec - 1] = pos;
    recaux->rec = recaux2;
    recaux2 = NULL;

    for (i = idx->n_recs - 1; i > k; i--)
      idx->recs[i] = idx->recs[i - 1];

    idx->recs[k] = recaux;
    recaux = NULL;

    return idx->n_recs;

  }

}

long *index_get(index_t *idx, int key, int* nposs) {

  int k = 0;

  if (!idx || !nposs) return NULL;

  k = binary(idx->recs, idx->n_recs-1, key);

  if (k < 0 || k >= idx->n_recs) return NULL;
  else{
    *nposs = idx->recs[k]->n_rec;
    return idx->recs[k]->rec;
  }

  return NULL;
}

void index_close(index_t *idx) {

  int i = 0;

  if (idx){
    fclose(idx->newfile);
    for (i = 0; i < idx->n_recs; i++){
      free(idx->recs[i]->rec);
    }
    if (idx->recs)free(idx->recs);
    free(idx);
    return;
  }

  return;
}

long *index_get_order(index_t *index, int n, int *key, int* nposs) {

  long *ppos = 0;

  if (!index || n < 0 || !key || !nposs) return NULL;

  if (n > index->n_recs - 1) return NULL;

  *key = index->recs[n]->key;
  *nposs = index->recs[n]->n_rec;

  return index->recs[n]->rec;
}