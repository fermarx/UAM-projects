/**
 * @fichero: ejercicio_shell.c
 * @author: Miguel Arnáiz Montes    miguel.arnaiz@estudiante.uam.es
 * @author: S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
 * @group: 2291_07
 * @date: 22/02/2020
 * 
 * @description:Tendrá un bucle principal que pida una línea al usuario para cada comando,
 * hasta leer EOF de la entrada estándar. Se puede introducir manualmente EOF
 * en la entrada est´andar mediante la combinación de teclas Ctrl + D . Para leer
 * líneas se puede usar fgets o getline.
 * Cada línea deberá trocearse para separar el ejecutable (primer argumento) y los
 * argumentos del programa (todos los argumentos, incluido el propio ejecutable).
 * Esta tarea de troceado puede realizarse ayudándose de funciones de librería
 * como strtok o, si se desea, se puede usar wordexp, que además realiza las
 * tareas adicionales que haría la shell (expandir variables de entorno, permitir
 * comillas, etc.).
 * Cada comando debe ejecutarse realizando un fork seguido de un exec en el
 * hijo.
 * El proceso padre debe esperar al hijo y a continuación imprimir por la salida de errores Exited with value <valor> o Terminated by signal <señal>, en
 * función de cómo terminó el hijo.
 * A continuación se realizará la siguiente iteración del bucle, leyendo el siguiente
 * comando
*/


// C Program to design a shell in Linux 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

#define LINE 1024

int main(){

    char line[LINE], *command[100],*path= "/bin/", filepath[20]; /*/bin/ porque vemos que están la mayoría de comandos ahí*/
    int n, i = 0;

    while(1){

        printf("exercise 10:$ ");

        if(!fgets(line, LINE, stdin)) break; /*if CTRL+D break*/

        char *tok = strtok(line," "); /*queremos que si encuentra un espacio lo detecte como un comando diferente*/
        for(i = 0; tok != NULL; i++){
            command[i] = tok; /*guarda en el array los comandos escritos por el usuario*/
            tok = strtok(NULL," ");
        }
        command[i]=NULL; /*el ultimo hueco tiene que estar vacío para que exec funcione*/

        strcpy(filepath, path);           /*copia el directorio donde quiere que se ejecute la shell*/
        strcat(filepath, command[0]);            /*añade el comando al path*/

        for(i = 0; i < strlen(filepath); i++)    /*elimina el comando*/
            if(filepath[i] == '\n') filepath[i]='\0';
        
        int pid = fork();

        if(pid < 0){
            perror("Fork");
            exit(EXIT_FAILURE);
        }else if(pid == 0){
            execvp(filepath, command);
            fprintf(stderr, "Terminated by signal %s\n", *command);
        }else if (pid > 0){
            wait(NULL);
            printf("Exited with value %i\n", pid);
        }

    }
}