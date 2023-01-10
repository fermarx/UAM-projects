/**
 * @fichero: ejercicio_hilos.c
 * @author: Miguel Arnáiz Montes    miguel.arnaiz@estudiante.uam.es
 * @author: S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
 * @group: 2291_07
 * @date: 22/02/2020
 * 
 * @description: Creará tantos hilos como se le indique por parámetro.
 * Cada hilo esperará un número aleatorio de segundos entre 0 y 10 inclusive, que
 * será generado por el hilo principal. Despuás realizará el cálculo x^3,
 * donde x será el námero del hilo creado. Por último devolverá el resultado del cálculo en un nuevo
 * entero, reservado dinámicamente.
 * El hilo principal deberá esperar a que todos los hilos terminen e imprimir todos los
 * resultados devueltos por los hilos.
 * Como la función pthread_create solo admite el paso de un único parúmetro habrú
 * que crear un struct con ambos parámetros (tiempo de espera y valor de x).
 * El programa deberá finalizar correctamente liberando todos los recursos utilizados.
 * Deberá asimismo controlar errores, y terminar imprimiendo el mensaje de error
 * correspondiente si se produce alguno.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    int tEspera;
    int x;   
} ej;
 
void *cubo(void * est){
    ej* aux = (ej*) est;
    int *res=NULL;
    sleep(aux->tEspera);

    res = (int*)malloc(sizeof(int));
    if(!res){
        exit(EXIT_FAILURE);    
    }

    *res= (aux->x)*(aux->x)*(aux->x);

    pthread_exit(res);
}
 
int main(int argc, char **argv){
 
  
    ej *estructura=NULL;
    int i, n, error, *resultados;
    pthread_t *h;
 
    n = atoi(argv[1]);
 
    if(argc!=2 || n<1){
        printf("Numero de argumentos incorrecto\n");
        exit(EXIT_FAILURE);
    }
 
    estructura = (ej*)malloc(sizeof(ej)*n);
    if(!estructura){
       perror("");
       exit(EXIT_FAILURE);
    }

    h = (pthread_t*)malloc(sizeof(pthread_t)*n);
    if(!h){
        perror("");
        free(estructura);
        exit(EXIT_FAILURE);
    }
 
    srand(time(NULL));
 
    for(i=0 ; i<n ; i++){
        estructura[i].tEspera = rand()%11;
        estructura[i].x = i+1;
        error= pthread_create(&h[i], NULL, cubo, &estructura[i]);
        if(error!=0){
            perror("");
            free(estructura);
            free(h);
            exit(EXIT_FAILURE);
        }
    }


   for(i=0 ; i<n ; i++){
        pthread_join(h[i], (void**)&resultados);
        printf("%d\n", *resultados);
        free(resultados);
    }

    free(estructura);
    free(h);
 
    exit(EXIT_SUCCESS);
}