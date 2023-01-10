#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "arqo3.h"

void compute(tipo **matrixA, tipo **matrixB, tipo **matrixC, int n);

int main( int argc, char *argv[]){
    int n;
    tipo **a = NULL, **b = NULL, **res = NULL;
    struct timeval fin,ini;

    printf("Word size: %ld bits\n",8*sizeof(tipo));

    if( argc!=2 ){
        printf("Error: ./%s <matrix size>\n", argv[0]);
        return -1;
    }
    n=atoi(argv[1]);
    a=generateMatrix(n);
    if( !a ){
        return -1;
    }
    b=generateMatrix(n);
    if(!b){
        freeMatrix(a);
        return -1;
    }
    res=generateEmptyMatrix(n);
    if(!res){
        freeMatrix(a);
        freeMatrix(b);
        return -1;
    }



    gettimeofday(&ini,NULL);

    /* Main computation */
    compute(a, b, res, n);
    /* End of computation */

    gettimeofday(&fin,NULL);
    printf("Execution time: %f\n", ((fin.tv_sec*1000000+fin.tv_usec)-(ini.tv_sec*1000000+ini.tv_usec))*1.0/1000000.0);

    freeMatrix(a);
    freeMatrix(b);
    freeMatrix(res);
    return 0;
}


void compute(tipo** matrixA,tipo** matrixB,tipo** matrixC, int n){
    tipo sum = 0;
    int i,j, k;

    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            sum = 0;
            for(k=0;k<n;k++){
                sum += matrixA[i][k] * matrixB[k][j];
            }
            matrixC[i][j] = sum;
        }
    }
}
