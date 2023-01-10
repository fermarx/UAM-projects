/**
 * @fichero: ejercicio_pipes.c
 * @author: Miguel Arnáiz Montes    miguel.arnaiz@estudiante.uam.es
 * @author: S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
 * @group: 2291_07
 * @date: 22/02/2020
 * 
 * @description:El proceso inicial debe crear dos procesos hijos.
 * Mediante tuberías, el proceso padre se debe comunicar con uno de sus hijos para
 * leer un número aleatorio x que generará dicho proceso hijo, y que enviará al padre
 * a través de una tubería.
 * Este primer proceso hijo, antes de finalizar, debe imprimir por pantalla el número
 * aleatorio que ha generado.
 * Una vez que el proceso padre tenga el número aleatorio del primer hijo, se lo enviará
 * al segundo proceso hijo a través de otra tubería distinta.
 * Este segundo proceso hijo debe leer el número de la tubería y escribirlo en el fichero
 * “numero_leido.txt”, usando las funciones que suministra el Sistema Operativo para
 * ello. Se puede usar la función dprintf para hacer escritura formateada usando un
 * descriptor de fichero.
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int main(){

    int fd1[2], fd2[2];
    float x, read_num, read_num2;

    int pipe_status1 = pipe(fd1);
	if(pipe_status1 == -1){
		perror("pipe 1");
		exit(EXIT_FAILURE);
	}

    int pipe_status2 = pipe(fd2);
	if(pipe_status2 == -1){
		perror("pipe 2");
		exit(EXIT_FAILURE);
	}

	pid_t child1id = fork();
    if (child1id < 0){
        perror("fork 1");
		exit(EXIT_FAILURE);
    } else if (child1id == 0){

        close(fd1[0]);

        srand(time(NULL));
        x = rand();

        sleep(0.1);

        ssize_t nbytes = write (fd1[1], &x, sizeof(x));
        if(nbytes == -1){
			perror("write 1");
			exit(EXIT_FAILURE);
		}

        printf("%.3f", x);

        exit(EXIT_SUCCESS);
    } else if (child1id > 0){
        
        close(fd1[1]);

        ssize_t nbytes = read (fd1[0], &read_num, sizeof(read_num));
        if(nbytes == -1){
            perror("read 1");
            exit(EXIT_FAILURE);
        }

        wait(NULL);
    }

    pid_t child2id = fork();
    if (child2id < 0){
        perror("fork 2");
		exit(EXIT_FAILURE);
    } else if (child2id == 0){
        
        close(fd2[1]);

        ssize_t nbytes = read (fd2[0], &read_num2, sizeof(read_num2));
        if(nbytes == -1){
            perror("read 2");
            exit(EXIT_FAILURE);
        }

        FILE *pf = NULL;

        pf = fopen("numero_leido.txt", "w");
        if (!pf){
            perror("opening file");
            exit(EXIT_FAILURE);
        }

        int characters = fprintf(pf, "%.3f", read_num2);
        if (characters < 0){
            perror("write in file");
            exit(EXIT_FAILURE);
        }

        fclose(pf);

        exit(EXIT_SUCCESS);
    } else if( child2id > 0){
        
        close(fd2[0]);

        ssize_t nbytes = write (fd2[1], &read_num, sizeof(read_num));
        if(nbytes == -1){
			perror("write 2");
			exit(EXIT_FAILURE);
		}

        wait(NULL);
        exit(EXIT_SUCCESS);
    }

    return 0;
}