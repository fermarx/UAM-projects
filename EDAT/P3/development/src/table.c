/*
    Project: EDAT Lab 3 test program
    File:    table.h
    Author:  Simone Santini
    Rev.     1.0
    Date:    10/13/2019
*/
#include "table.h"
#include "type.h"

struct table_ {
    FILE *newfile;
    type_t *types;
    int ncols;
    long pos;
    void **values;
};

int table_create(char* path, int ncols, type_t* types) {
    
    FILE *newfile = NULL;
    int write = 0;

    if (!path || ncols < 0 || !types) return 0;

    newfile = fopen (path, "wb");
    if (!newfile) return 0;

    write = fwrite(&ncols, sizeof(int), 1, newfile);
    if (write == -1){
        fclose(newfile);
        return 0;
    }

    write = fwrite(types, sizeof(type_t), ncols, newfile);
    if (write == -1){
        fclose(newfile);
        return 0;
    }   

    fclose(newfile);

    return 1;
}

table_t* table_open(char* path) {

    table_t *return_path = NULL;
    int read = 0;

    if (!path) return NULL;

    return_path = (table_t*)malloc(1 * sizeof(table_t));
    if (!return_path) return NULL;

    return_path->newfile = NULL;
    return_path->ncols=0;
    return_path->types=NULL;
    return_path->values=NULL;
    return_path->pos = 0;

    return_path->newfile = fopen(path, "r+");
    if(!return_path->newfile){
        free(return_path);
        return NULL;
    }

    read = fread(&return_path->ncols, sizeof(int), 1, return_path->newfile);
    if (read == -1){
        fclose(return_path->newfile);
        free(return_path);
        return NULL;
    }

    return_path->types = malloc((return_path->ncols) * sizeof(type_t));
    if(!return_path->types){
        fclose(return_path->newfile);
        free(return_path);
        return NULL;
    }

    read = fread(return_path->types , sizeof(type_t), return_path->ncols, return_path->newfile);
    if (read == -1){
        free(return_path->types);
        fclose(return_path->newfile);
        free(return_path);
        return NULL;
    }

    return_path->values = (void**) malloc(return_path->ncols*sizeof(void*));
    if (!return_path->values){
        free(return_path->types);
        fclose(return_path->newfile);
        free(return_path);
        return NULL;
    }
    for (int i=0; i<return_path->ncols; i++)
        return_path->values[i] = NULL;

    return_path->pos = ftell(return_path->newfile);

    return return_path; 
}

void table_close(table_t* table) {

    if (table){
        fclose(table->newfile);
        free (table->types);
        if (table->values) free(table->values);
        free(table);
        return;   
    }

    return;
}

int table_ncols(table_t* table) {
    int ncol = 0;

    if (!table) return -1;

    ncol = table->ncols;
    if (ncol < 0) return -1;

    return ncol;
}

type_t *table_types(table_t* table) {

    if(!table) return NULL;

    return table->types;
}

long table_first_pos(table_t* table) {

    if (!table) return -1; 

    return table->pos;
}

long table_last_pos(table_t* table) {

    if (!table) return 0;

    if(fseek(table->newfile, 0, SEEK_END) == -1) return 0;

    return ftell(table->newfile);
}

long table_read_record(table_t* t, long pos) { 

    size_t s;
    int i = 0, read = 0;

    if(!t || pos < 0) return -1;

    if (fseek(t->newfile, pos, SEEK_SET) == -1) return -1; 

    /*Free the older buffer if it exist*/   
    if(t->values[0] != NULL){
        free(t->values[0]);
        t->values[0] = NULL;
    }

    for (int i=0; i < t->ncols; i++) {
        if (t->values[i]) free(t->values[i]);
        if((fread(&s, sizeof(size_t), 1, t->newfile)) < 1) return -1;
        t->values[i] = (void *) malloc(s*sizeof(char));
        read = fread(t->values[i], sizeof(char), s, t->newfile);
        if(read == -1) return -1;
    }

    return ftell(t->newfile);
}

void *table_get_col(table_t* table, int col) {

    if (!table || col < 0) return NULL;

    return  table->values[col];
}

int table_insert_record(table_t* t, void** values) {

    int i, pos;
    size_t size;

    if(!t || !values) return 0;

    for(i=0; i< t->ncols; i++) {
        size = value_length(t->types[i], values[i]);
        fwrite(&size, sizeof(size_t), 1, t->newfile);
        fwrite(values[i], value_length(t->types[i], values[i]), 1, t->newfile);
    }

    return 1;
}