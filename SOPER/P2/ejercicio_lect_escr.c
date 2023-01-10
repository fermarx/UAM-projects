#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>


#define SEM1_NAME "/sem_escritura"
#define SEM2_NAME "/sem_lectura" 

int static sigintCount=0;

void manejador_SIGINT(int sig){
    sigintCount=1;
}

void manejador_SIGTERM(int sig){
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv){

    int n_read, secs, i, lectores = 0, actual_pid;
    pid_t *pid = NULL;
    sem_t *sem_escritura = NULL, *sem_lectura = NULL;
    FILE *pf = NULL;
    struct sigaction act;


    if (argc < 3){
        printf("Enter <./executable> <n_childs> <time>\n");
        exit(EXIT_FAILURE);
    }

    n_read = atoi(argv[1]);
    secs = atoi(argv[2]);

    pid = (pid_t*)malloc(sizeof(pid_t)*n_read);
    if(!pid){
        perror("Malloc: ");
        exit(EXIT_FAILURE);
    }

    sigemptyset(&(act.sa_mask));
    act.sa_flags = 0;
    act.sa_handler = manejador_SIGINT;
    if(sigaction(SIGINT, &act, NULL) < 0){
        perror("handler");
        exit(EXIT_FAILURE);
    }

    act.sa_handler = manejador_SIGTERM;
    if(sigaction(SIGTERM, &act, NULL) < 0){
        perror("handler");
        exit(EXIT_FAILURE);
    }

    if((sem_escritura = sem_open(SEM1_NAME, O_CREAT, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED){
        perror ("sem_open") ;
        exit ( EXIT_FAILURE ) ;
    }
    if((sem_lectura = sem_open(SEM2_NAME, O_CREAT, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED){
        perror ("sem_open") ;
        exit ( EXIT_FAILURE ) ;
    }

    for(i=0; i<n_read; i++){
        pid[i]=fork();
        if(pid[i]<0){
            perror("Fork: ");
            free (pid);
            exit(EXIT_FAILURE);
        }
        if(!pid[i]) break;
        if(i==n_read - 1) break;
    }

    if(!pid[i]){/*hijo lector*/
        
        free(pid);
        actual_pid = getpid();

        while(1){
            sem_wait(sem_lectura);
            lectores++;
            if(lectores == 1)
                sem_wait(sem_escritura);
            /*sem_post(sem_lectura);*/
            
            printf("R-INI <%i> ", actual_pid);
            sleep(1);
            printf("R-FIN <%i>\n", actual_pid);

            /*sem_wait(sem_lectura);*/
            lectores--;
            if(lectores == 0)
                sem_post(sem_escritura);
            sem_post(sem_lectura);
        }

    }else{/*padre escritor*/

        actual_pid = getpid();

        while(sigintCount==0){
            printf("W-INI <%i> ", actual_pid);
            sleep(1);
            printf("W-FIN <%i>\n", actual_pid);
            sleep(secs);
        }

        for(i=0; i<n_read; i++){
            kill(pid[i], SIGTERM);
        }
        free(pid);
        sem_close(sem_lectura);
        sem_close(sem_escritura);
        sem_unlink(SEM1_NAME);
        sem_unlink(SEM2_NAME);
    }
    
    
    exit(EXIT_SUCCESS);
}