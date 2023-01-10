/**
 * @fichero: ejercicio_arbol.c
 * @author: Miguel Arnáiz Montes    miguel.arnaiz@estudiante.uam.es
 * @author: S. Xiao Fernández Marín	sofiax.fernandez@estudiante.uam.es
 * @group: 2291_07
 * @date: 22/02/2020
 * 
 * @description: El proceso padre genera un proceso hijo, que a su vez generará otro hijo, y así hasta llegar
 * a NUM_PROC procesos en total. El programa debe garantizar que cada padre espera a que
 * termine su hijo, y no quede ning´un proceso huérfano.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv){

    int num_proc, i;
	pid_t pid;

    if(argc < 2){
        printf ("Enter a number.\n");
        return 1;
    }

    num_proc = atoi(argv[1]);

	for( i = 0; i <= num_proc; i++){

		pid = fork();
	
		if(pid <  0){
			perror("fork");
			exit(EXIT_FAILURE);
		}else if(pid ==  0){
			printf("Hijo %d\n", i);
		}else if(pid >  0){
			printf("Padre %d\n", i);
			wait(NULL);
		}
	}
	wait(NULL);
	exit(EXIT_SUCCESS);
}
