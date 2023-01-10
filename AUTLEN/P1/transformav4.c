#include "transforma.h"
#include <string.h>
#define MAX_SIMBOLOS 10
#define MAX_ESTADOS 10
#define MAX_NOMBRE 20


typedef struct _estado{   
    int id;
    int estados[MAX_ESTADOS];
    int num_estados;
    int tipo;
}Estado;

typedef struct _array{  
    Estado estados[MAX_ESTADOS];
    int num_estados;
}Array;

void initEstado(Estado *e){
    int i;
    e->id=-1;
    for(i=0; i<MAX_ESTADOS; i++){
        e->estados[i]=0;
    }
    e->num_estados=0;
    e->tipo=NORMAL;
}

void initArray(Array *a){
    int i;
    a->num_estados=0;
    for(i=0; i<MAX_ESTADOS; i++){
        initEstado(&a->estados[i]);
    }
}

void insertarEstadoEnEstado(Estado *e, int nuevo_estado){
    int i;
    if(e->num_estados == MAX_ESTADOS) return;
    
    for(i=0; i<e->num_estados; i++){
        if(e->estados[i]==nuevo_estado){
            return;
        }
    }
    e->estados[e->num_estados] = nuevo_estado;
    e->num_estados++;
}

char* crearNombreEstado(AFND * afnd, Estado *e){
    char *nombre=NULL;
    int i;

    nombre = (char*)malloc(MAX_NOMBRE*sizeof(char));
    if(!nombre) return NULL;

    strcpy(nombre, AFNDNombreEstadoEn(afnd, e->estados[0]));
    for(i=1; i<e->num_estados; i++){
        strcat(nombre, AFNDNombreEstadoEn(afnd, e->estados[i]));
    }
    return nombre;
}

/*Devuelve 1 si existe y 0 si no existe*/
int existeEstado(Array *a, Estado *e){
    int i, j, k, count;

    if(!a || !e || a->num_estados == MAX_ESTADOS) return 0;

    for (i=0, count=0; i<a->num_estados; i++){
        if(a->estados[i].num_estados == e->num_estados){
            for(j=0; j<a->estados[i].num_estados; j++){
                for(k=0; k<e->num_estados; k++){
                    if(a->estados[i].estados[j] == e->estados[k]){
                        count++;
                        break;
                    }
                }
            }
        }

        /*Ha encontrado un estado igual*/
        if(count==e->num_estados){
            return 1;
        }
        /*Si no lo encuentra reseteamos count y vamos con el siguiente estado*/
        count=0;
    }
    return 0;
}

Estado copiarEstado(Estado *e){
    Estado nuevo;
    int i;

    initEstado(&nuevo);

    for(i=0; i < e->num_estados; i++){
        nuevo.estados[i]=e->estados[i];
    }

    nuevo.id = e->id;
    nuevo.tipo = e->tipo;
    nuevo.num_estados = e->num_estados;

    return nuevo;
}

void insertaEstadoEnArray(Array *a, Estado *e){
    Estado cpy;

    if(!a || !e || a->num_estados==MAX_ESTADOS) return;

    if(existeEstado(a, e)==1) return;

    cpy = copiarEstado(e);

    a->estados[a->num_estados] = cpy;
    a->num_estados++;
}

void buscarTransicionesLambda(int n_estados, int n_simbolos, int matriz[MAX_ESTADOS][MAX_SIMBOLOS][MAX_ESTADOS], int estado, Estado *e){
    int i, k, flag=0;

    for(k=0; k<n_estados; k++){

        if(matriz[estado][n_simbolos][k]==1){

            for(i=0, flag=0; i < e->num_estados; i++){
                if(e->estados[i] == k){
                    flag=1;
                    break;
                }
            }

            if(flag==0){
                insertarEstadoEnEstado(e, k);
                buscarTransicionesLambda(n_estados, n_simbolos, matriz, k, e);
            }
            
        }
    }
}

/*Si es INICIAL_Y_FINAL returneamos ya que no va a cambiar más*/
void determinarTipoEstado(AFND *afnd, Estado *e){
    int i;

    for(i=0; i<e->num_estados; i++){
        
        if(e->tipo==FINAL){
            if(e->tipo == INICIAL || e->tipo == INICIAL_Y_FINAL){
                e->tipo = INICIAL_Y_FINAL;
                return;
            }
        }else if(e->tipo==INICIAL){
            if(AFNDTipoEstadoEn(afnd, e->estados[i]) == FINAL || AFNDTipoEstadoEn(afnd, e->estados[i]) == INICIAL_Y_FINAL){
                e->tipo = INICIAL_Y_FINAL;
                return;
            }
        }else if(e->tipo==NORMAL){
            if(AFNDTipoEstadoEn(afnd, e->estados[i]) == INICIAL){
                e->tipo = INICIAL;
            }else if(AFNDTipoEstadoEn(afnd, e->estados[i]) == FINAL){
                e->tipo = FINAL;
            } else if(AFNDTipoEstadoEn(afnd, e->estados[i]) == INICIAL_Y_FINAL){
                e->tipo = INICIAL_Y_FINAL;
                return;
            }
        }
        
    }
}

/*Devuelve el indice del estado en el array, -1 si no lo encuentra*/
int buscaEstadoEnArray(Estado*e, Array *a){
    int i, j, k, count;

    if(!e || !a) return -1;
    
    for (i=0, count=0; i < a->num_estados; i++){
        if(a->estados[i].num_estados == e->num_estados){
            for(j=0; j<a->estados[i].num_estados; j++){
                for(k=0; k<e->num_estados; k++){
                    if(a->estados[i].estados[j] == e->estados[k]){
                        count++;
                        break;
                    }
                }
            }
        }

        /*Ha encontrado un estado igual*/
        if(count==a->estados[i].num_estados){
            return i;
        }
        /*Si no lo encuentra reseteamos count y vamos con el siguiente estado*/
        count=0;
    }

    return -1;
}


AFND * AFNDTransforma(AFND * afnd){

    int fila = 0, columna = 0, capa = 0, n_estados = 0, n_simbolos = 0, i, j, k, l, encontrado;
    int matriz[MAX_ESTADOS][MAX_SIMBOLOS][MAX_ESTADOS], flag;
    char *nombre;
    Array array;
    Estado estado;
    AFND *afd;


    n_estados = AFNDNumEstados(afnd);
    n_simbolos = AFNDNumSimbolos(afnd);
    /**
     *  matriz del afnd dado.
     *  n_estados * n_simbolos+1 * n_estados
     */
    for(fila = 0; fila < n_estados; fila++){
        for(columna = 0; columna < (n_simbolos + 1); columna++){
            for(capa = 0; capa < n_estados; capa++){
                
                if(columna == n_simbolos){
                    matriz[fila][columna][capa] = AFNDCierreLTransicionIJ(afnd, fila, capa);
                } else {
                    matriz[fila][columna][capa] = AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, fila, columna, capa);
                }
            }
        }
    }

    /**
     * creacion de cada estado que va a ir en el afd
     */
    initArray(&array);
    
    /*Creamos el estado inicial (suponemos que el estado de indice 0 es el estado inicial)*/
    initEstado(&estado);
    insertarEstadoEnEstado(&estado, 0); 
    buscarTransicionesLambda(n_estados, n_simbolos, matriz, 0, &estado);
    determinarTipoEstado(afnd, &estado);
    insertaEstadoEnArray(&array, &estado);
    
    for(i=0; i<array.num_estados; i++){
        for(columna=0; columna<n_simbolos; columna++){
            initEstado(&estado);
            for(j=0; j<array.estados[i].num_estados; j++){
                for(capa=0; capa<n_estados; capa++){
                    if(AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, array.estados[i].estados[j], columna, capa) == 1){
                        insertarEstadoEnEstado(&estado, capa);
                        buscarTransicionesLambda(n_estados, n_simbolos, matriz, capa, &estado);
                    }
                }
            }
            determinarTipoEstado(afnd, &estado);
            insertaEstadoEnArray(&array, &estado);
        }
    }


    /**
     *  matriz afd
     */
    afd = AFNDNuevo("afd", array.num_estados, n_simbolos);
    /* inserta los simbolos que se utilizan en el afnd */
    for ( i = 0; i <  n_simbolos; i++) {
        AFNDInsertaSimbolo(afd, AFNDSimboloEn(afnd, i));
    }

    /* inserta los nuevos estados que se han creado */
    for (i = 0; i < array.num_estados; i++) {
        nombre = crearNombreEstado(afnd, &array.estados[i]);
        AFNDInsertaEstado(afd, nombre, array.estados[i].tipo);
        free(nombre);
    }

    encontrado=-1;
    for(i=0; i<array.num_estados; i++){
        for(columna=0; columna<n_simbolos; columna++){
            initEstado(&estado);
            for(j=0; j<array.estados[i].num_estados; j++){
                for(capa=0; capa<n_estados; capa++){
                    if(AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, array.estados[i].estados[j], columna, capa) == 1){
                        insertarEstadoEnEstado(&estado, capa);
                        buscarTransicionesLambda(n_estados, n_simbolos, matriz, capa, &estado);
                    }
                }
            }
            encontrado = buscaEstadoEnArray(&estado, &array);
            if(encontrado!=-1){
                char *nombre1 = crearNombreEstado(afnd, &array.estados[i]);
                char *nombre2 = crearNombreEstado(afnd, &array.estados[encontrado]);

                AFNDInsertaTransicion(afd, nombre1, AFNDSimboloEn(afnd, columna), nombre2);
                free(nombre1);
                free(nombre2);
            }
            encontrado=-1;

        }
    }

    return afd;
}


