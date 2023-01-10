#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

#define SEM_NAME "/sem_1"
#define FILE_NAME "data.txt"

int  N;
pid_t *pid;
int alarmCount=0;

void manejador_SIGALRM(int sig) {
    alarmCount=1;
}

void manejador_SIGTERM(int sig) {
    printf("Finalizado %d\n", getpid());
    exit(EXIT_SUCCESS);
}

void manejador_SIGUSR2(int sig) {
    
    int i, num_proc, sum_proc;
    sem_t *sem1 = NULL;
    FILE *pf=NULL;

    sem1 = sem_open(SEM_NAME, 0);

    sem_wait(sem1);
    pf = fopen(FILE_NAME, "r");
    if (!pf){
        perror("File.");
        exit(EXIT_FAILURE);
    }

    fscanf(pf, "%i", &num_proc);
    fscanf(pf, "%i", &sum_proc);

    fclose(pf);
    sem_post(sem1);
    

    if(num_proc == N && sum_proc > 0){
        fprintf (stdout, "Han acabado todos, resultado: %i\n", sum_proc);

        for(i=0; i<N; i++){
            kill(pid[i], SIGTERM);
        }

        sem_close(sem1);
        sem_unlink(SEM_NAME);
        free(pid);
        exit(EXIT_SUCCESS);
    }
    
}

int main(int argc, char **argv){

    int T, i, count = 0, num_proc = 0, sum_proc = 0;
    sigset_t set;
    struct sigaction act;
    sem_t *sem1 = NULL;
    FILE *pf = NULL;

    if (argc < 3){
        printf ("./<executable> <n_hijos> <tiempo_suspension>\n");
        exit(EXIT_FAILURE);
    }

    N = atoi(argv[1]);
    T = atoi(argv[2]);

    sigemptyset(&(act.sa_mask));
    act.sa_flags = 0;
    act.sa_handler = manejador_SIGUSR2;
    if (sigaction(SIGUSR2, &act, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE); 
    }

    act.sa_handler = manejador_SIGTERM;
    if (sigaction(SIGTERM, &act, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE); 
    }

    if((sem1 = sem_open(SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED){
        perror ("sem_open") ;
        exit ( EXIT_FAILURE ) ;
    }

    pid = (pid_t*)calloc(sizeof(pid_t),N);
    if(!pid){
        perror("Malloc: ");
        exit(EXIT_FAILURE);
    }

    sem_wait(sem1);
    pf = fopen(FILE_NAME, "w");
    if (!pf){
        perror("File.");
        exit(EXIT_FAILURE);
    }

    fprintf(pf, "0\n0");
    fclose(pf);
    sem_post(sem1);
    
    
    for(i=0; i<N; i++){
        pid[i]=fork();
        if(pid[i]<0){
            perror("Fork: ");
            free (pid);
            exit(EXIT_FAILURE);
        }
        if(!pid[i]) break;
        if(i==N-1) break;
    }
    
    if(!pid[i]){ /*hijo*/

        free(pid); 

        for(i=1; i<getpid()/10; i++){
            count+= i; 
        }
        
        printf("PID del hijo: %i. Total: %i\n", getpid(), count);
        
        sem_wait(sem1);
        pf = fopen(FILE_NAME, "r");
        if (!pf){
            perror("File.");
            exit(EXIT_FAILURE);
        }

        fscanf(pf, "%i", &num_proc);
        fscanf(pf, "%i", &sum_proc);

        fclose(pf);
        sem_post(sem1);

        num_proc++;
        sum_proc = sum_proc + count;

        sem_wait(sem1);
        pf = fopen(FILE_NAME, "w");
        if (!pf){
            perror("File.");
            exit(EXIT_FAILURE);
        }
        fprintf(pf, "%i\n", num_proc);
        fprintf(pf, "%i", sum_proc);
        fclose(pf);
        sem_post(sem1);

        kill(getppid(), SIGUSR2);

        sigfillset(&set);
        sigdelset(&set, SIGTERM);

        sigsuspend(&set);

    }else{ /*padre*/
        
        sigemptyset(&(act.sa_mask));
        act.sa_flags = 0;

        act.sa_handler = manejador_SIGALRM;
        if (sigaction(SIGALRM, &act, NULL) < 0) {
            perror("sigaction");
            exit(EXIT_FAILURE); 
        }

        if (alarm(T)) {
            fprintf(stderr, "Alarm.\n");
        }
    
        sigfillset(&set);
        sigdelset(&set, SIGALRM);
        sigdelset(&set, SIGUSR2);

        while(alarmCount!=1){
            sigsuspend(&set);
        }

        printf("Falta trabajo\n");

        for(i=0; i<N; i++){
            kill(SIGTERM, pid[i]);
        }
        free(pid);
        sem_close(sem1);
        sem_unlink(SEM_NAME);
        
    }


    exit(EXIT_SUCCESS);
}