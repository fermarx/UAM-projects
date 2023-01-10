/**
 * @fichero: ejercicio_shell_spawn.c
 * @author: Miguel Arnáiz Montes    miguel.arnaiz@estudiante.uam.es
 * @author: S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
 * @group: 2291_07
 * @date: 22/02/2020
 * 
 * @description:Tendrá un bucle principal que pida una línea al usuario para cada comando,
 * hasta leer EOF de la entrada estándar. Se puede introducir manualmente EOF
 * en la entrada estandar mediante la combinación de teclas Ctrl + D . Para leer
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
 * 
 * 
 * Se reemplaza fork + exec por posix_spawn y posix_spawnp.
*/


// C Program to design a shell in Linux 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <spawn.h>
#include <wordexp.h>

#define LINE 1024

extern char **environ;

int main(){

    char line[LINE], *command = NULL, *path= "/bin/sh", filepath[20]; /*/bin/ porque vemos que están la mayoría de comandos ahí*/
    int n, i = 0;
    pid_t pid;
    wordexp_t input;


    printf("exercise 10:$ ");

    command = (char*)calloc(100, sizeof(char));
    if (!command) return 1;

    if(!fgets(line, LINE, stdin)) return 1; /*if CTRL+D break*/

    char *tok = strtok(line," "); /*queremos que si encuentra un espacio lo detecte como un comando diferente*/
    for(i = 0; tok != NULL; i++){
        command[i] = *tok; /*guarda en el array los comandos escritos por el usuario*/
        tok = strtok(NULL," ");
    }
    command[i] = '\0';

    if(wordexp(command, &input, 0) != 0){
        perror("command");
        exit (EXIT_FAILURE);
    }

    for(i = 0; i < strlen(filepath); i++)    /*elimina el comando*/
        if(filepath[i] == '\n') filepath[i]='\0';
    
    int posixerr = posix_spawn(&pid, path, NULL, NULL, input.we_wordv, environ);
    if (posixerr == 0){
        if (waitpid(pid, &posixerr, 0) != -1){
            printf("Exited with status %i", posixerr);
        }else perror("posix");
    }else printf("posix_spawn: %s", strerror(posixerr));

    wordfree(&input);
    free(command);

    return 0;
}