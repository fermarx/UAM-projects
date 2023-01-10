#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


int main(int argc, char** argv){

    int sign;
    __pid_t pid;

    if(argc<3){
        printf("Introduce <identificador de seĂąal> <pid>\n");
        return 1;
    }
    
    sign=atoi(argv[1]);
    pid=atoi(argv[2]);
    
    if(kill(pid, sign)==-1){
        perror("kill: ");
        return 1;
    }

    return 0;
}