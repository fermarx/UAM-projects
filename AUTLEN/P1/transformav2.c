#include "transforma.h"
#include <string.h>
#define MAX_SIMBOLOS 1024
#define MAX_ESTADOS 1024

typedef struct {
    char* nombre;
    int tipo;
}Estado;

typedef struct {
    char* simbolos[MAX_SIMBOLOS];
    Estado estados[MAX_ESTADOS];
    int num_estados;
}NuevoAFD;

AFND * AFNDTransforma(AFND * afnd){
    int n_estados=0, n_simb=0;
    int i, j, k, x;//counters
    
    /*Inicializamos estructura*/
    NuevoAFD organizador;
    organizador.num_estados = 0;
    for(i=0; i<MAX_SIMBOLOS; i++){
        organizador.simbolos[i]=NULL;
        organizador.estados[i].nombre=NULL;
        organizador.estados[i].tipo=-1;
    }

    n_estados = AFNDNumEstados(afnd);
    n_simb = AFNDNumSimbolos(afnd);

    /*Rellenamos la estructura con los simbolos y estados iniciales*/
    for(i=0; i<n_simb; i++){
        organizador.simbolos[i] = AFNDSimboloEn(afnd, i);
    }
    /*Añadimos lambda con posicion n_simb*/
    organizador.simbolos[i] = "λ";

    for(i=0; i<n_estados; i++){
        organizador.estados[i].nombre = AFNDNombreEstadoEn(afnd, i);
        organizador.estados[i].tipo = AFNDTipoEstadoEn(afnd, i);
        organizador.num_estados++;
    }

    /*Inicializamos la matriz de transiciones inicial a 0*/
    int matriz_transiciones[n_estados][n_simb+1][n_estados];
    for(i=0; i<n_estados; i++){
        for(j=0; j<n_simb; j++){
            for(k=0; k<n_estados; k++){
                matriz_transiciones[i][j][k]=0;
            }
        }
    }

    /*Asignamos 1 o 0 dependiendo de si podemos transicionar al del estado i al k con el simbolo j (todo con indices)*/
    for(i=0; i<n_estados; i++){
        for(j=0; j<n_simb+1; j++){
            for(k=0; k<n_estados; k++){
                if(j<n_simb){
                    matriz_transiciones[i][j][k]= AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, i, j, k);
                } else if (j==n_simb){
                    matriz_transiciones[i][j][k]= AFNDCierreLTransicionIJ(afnd, i, k);
                }
            }
        }
    }

    /*Creamos una nueva matriz para pasar de afnd a afd*/
    int matriz_afd[MAX_ESTADOS][n_simb][MAX_ESTADOS];
    for(i=0; i<MAX_ESTADOS; i++){
        for(j=0; j<n_simb; j++){
            for(k=0; k<MAX_ESTADOS; k++){
                matriz_afd[i][j][k]= 0;
            }
        }
    }

    /*Inicializamos nuevo afd para ir metiendo nuevos estados*/
    NuevoAFD def;
    def.num_estados = 0;
    for(i=0; i<MAX_SIMBOLOS; i++){
        def.simbolos[i]=NULL;
        def.estados[i].nombre=NULL;
        def.estados[i].tipo=-1;
    }

    for(i=0; i<n_simb; i++){
        def.simbolos[i] = AFNDSimboloEn(afnd, i);
    }

    Estado nuevoEstado;
    nuevoEstado.nombre=NULL;
    nuevoEstado.tipo=-1;

    int count=0;
    int transiciones[MAX_ESTADOS];
    for(i=0; i<MAX_ESTADOS; i++){
        transiciones[i]= -1;
    }


    for(i=0; i<n_estados; i++){
        j=n_simb;//aqui es donde esta lambda
            for(k=0, count=0; k<n_estados; k++){
                if(k==0){
                    transiciones[count]= i;
                    count++;
                }

                if(matriz_transiciones[i][j][k]==1){
                    transiciones[count]= k;
                    count++;
                }
            }
            /*Crea el nuevo nombre del estado concatenando nombres y poniendo*/
            nuevoEstado.tipo=NORMAL;
            nuevoEstado.nombre=NULL;
            for(x=0; x<count; x++){
                strcat(nuevoEstado.nombre, organizador.estados[transiciones[x]].nombre);
                if(nuevoEstado.tipo==FINAL){
                    if(organizador.estados[transiciones[x]].tipo == INICIAL || organizador.estados[transiciones[x]].tipo == INICIAL_Y_FINAL){
                        nuevoEstado.tipo = INICIAL_Y_FINAL;
                    }
                }else if(nuevoEstado.tipo==INICIAL){
                    if(organizador.estados[transiciones[x]].tipo == FINAL || organizador.estados[transiciones[x]].tipo == INICIAL_Y_FINAL){
                        nuevoEstado.tipo = INICIAL_Y_FINAL;
                    }
                }else if(nuevoEstado.tipo==NORMAL){
                    if(organizador.estados[transiciones[x]].tipo == INICIAL){
                        nuevoEstado.tipo = INICIAL;
                    }else if(organizador.estados[transiciones[x]].tipo == FINAL){
                        nuevoEstado.tipo = FINAL;
                    } else if(organizador.estados[transiciones[x]].tipo == INICIAL_Y_FINAL){
                        nuevoEstado.tipo = INICIAL_Y_FINAL;
                    }
                }
                
            }

            /*No funciona*/
            for(x=0; x<def.num_estados; x++){
                if(strcmp(nuevoEstado.nombre, def.estados[x].nombre)==0){
                    break;
                }
            }
            if(x==def.num_estados){
                strcpy(def.estados[x].nombre, nuevoEstado.nombre);
                def.estados[x].tipo = nuevoEstado.tipo;
                def.num_estados++;
            }

        
    }



/*
    AFND* afd = NULL;
    int i = 0, j = 0, k = 0, ***matrix = NULL, **matrix_lambda = NULL, *e_ini = NULL, *e_fin = NULL;
    int n_estados = 0, n_simb = 0;
    char **simbolo;

    if (!afnd) return NULL;

    /**
     * matriz del afnd dado.
     * 
     * cada fila es un estado diferente
     * cada columna es un simbolo
     * cada elemento de la tabla es un array de estados que el estado 
     *      de la fila puede visitar. Al poder tener en cada estado 
     *      diferentes estados a los que transitar hay que hacer un array
     *      de tres dimensiones.
    */
 /*   n_estados = AFNDNumEstados(afnd);
    n_simb = AFNDNumSimbolos(afnd);
    int matrixv2[n_estados][n_simb][n_estados];
    int estados_transicionables[n_estados];
    int count = 0;
    

    if(!matrix) return NULL;
    for (i = 0; i < n_estados; i++) {
        for (j = 0; j < n_simb; j++) {

            matrix[i][j][k] = AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, i, j, k);
            if(!matrix[i][j][k]){
                free(matrix);
                free(matrix[i]);
                free(matrix[i][j]);
                return NULL;
            }

            for (int count=0, k = 0; k < n_estados; k++) {
                if (matrixv2[i][j][k] == 1){
                    estados_transicionables[count] = k;
                    count++;
                }
            }
        }
    }


    /**
     * matriz con transiciones lambda.
    */
/*    AFND *c_afnd = AFNDCierraLTransicion(afnd);
    n_estados = AFNDNumEstados(c_afnd);
    matrix_lambda = (int**) malloc (n_estados*sizeof(int*));
    for (i = 0; i < n_estados; i++)
        matrix_lambda[i] = (int*) malloc(n_estados*sizeof(int));
    for (i = 0; i < n_estados; i++) {
        for (j = 0; j < n_estados; j++)
            matrix_lambda[i][j] = AFNDCierreLTransicionIJ(afnd, i, j);
    }

    /**
     * array con los estados iniciales.
    */
/*    n_estados = AFNDNumEstados(afnd);

    e_ini = (int*)malloc(n_estados*sizeof(int));
    for (i = 0; i < n_estados; i++) {
        if (AFNDTipoEstadoEn(afnd, i) == INICIAL)
            e_ini[i] = (AFNDTipoEstadoEn(afnd, i) == INICIAL);
        if (AFNDTipoEstadoEn(afnd, i) == INICIAL_Y_FINAL)
            e_ini[i] = (AFNDTipoEstadoEn(afnd, i) == INICIAL_Y_FINAL);
    }

    /**
     * array con los estados finales.
    */
 /*   e_fin = (int*)malloc(n_estados*sizeof(int));
    for (i = 0; i < n_estados; i++) {
        if (AFNDTipoEstadoEn(afnd, i) == FINAL)
            e_fin[i] = (AFNDTipoEstadoEn(afnd, i) == FINAL);
        if (AFNDTipoEstadoEn(afnd, i) == INICIAL_Y_FINAL)
            e_fin[i] = (AFNDTipoEstadoEn(afnd, i) == INICIAL_Y_FINAL);
    }

    /**
     * matriz con el alfabeto del autmata dado
    */
/*    simbolo = (int**)malloc(n_estados*sizeof(int*));
    for (i = 0; i < n_estados; i++)
        simbolo[i] = AFNDSimboloEn(afnd, i);*/


}