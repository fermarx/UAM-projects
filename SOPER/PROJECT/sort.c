#define _POSIX_C_SOURCE 200112L

#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <mqueue.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "sort.h"
#include "utils.h"

#define SHM_NAME "/shared_mem"
#define MQ_NAME "/mq_example"
#define SEM_NAME "/sem_name"
#define SEM_NAME1 "/sem_name1"
#define SEM_NAME2 "/sem_name2"
#define MAX_CHAR 1024

Status bubble_sort(int *vector, int n_elements, int delay) {
    int i, j;
    int temp;

    if ((!(vector)) || (n_elements <= 0)) {
        return ERROR;
    }

    for (i = 0; i < n_elements - 1; i++) {
        for (j = 0; j < n_elements - i - 1; j++) {
            /* Delay. */
            fast_sleep(delay);
            if (vector[j] > vector[j+1]) {
                temp = vector[j];
                vector[j] = vector[j + 1];
                vector[j + 1] = temp;
            }
        }
    }

    return OK;
}

Status merge(int *vector, int middle, int n_elements, int delay) {
    int *aux = NULL;
    int i, j, k, l, m;

    if (!(aux = (int *)malloc(n_elements * sizeof(int)))) {
        return ERROR;
    }

    for (i = 0; i < n_elements; i++) {
        aux[i] = vector[i];
    }

    i = 0; j = middle;
    for (k = 0; k < n_elements; k++) {
        /* Delay. */
        fast_sleep(delay);
        if ((i < middle) && ((j >= n_elements) || (aux[i] < aux[j]))){
            vector[k] = aux[i];
            i++;
        }
        else {
            vector[k] = aux[j];
            j++;
        }

        /* This part is not needed, and it is computationally expensive, but
        it allows to visualize a partial mixture. */
        m = k + 1;
        for (l = i; l < middle; l++) {
            vector[m] = aux[l];
            m++;
        }
        for (l = j; l < n_elements; l++) {
            vector[m] = aux[l];
            m++;
        }
    }

    free((void *)aux);
    return OK;
}

int get_number_parts(int level, int n_levels) {
    /* The number of parts is 2^(n_levels - 1 - level). */
    return 1 << (n_levels - 1 - level);
}

Status init_sort(char *file_name, Sort *sort, int n_levels, int n_processes, int delay) {
    char string[MAX_STRING];
    FILE *file = NULL;
    int i, j, log_data;
    int block_size, modulus;

    if ((!(file_name)) || (!(sort))) {
        fprintf(stderr, "init_sort - Incorrect arguments\n");
        return ERROR;
    }

    /* At most MAX_LEVELS levels. */
    sort->n_levels = MAX(1, MIN(n_levels, MAX_LEVELS));
    /* At most MAX_PARTS processes can work together. */
    sort->n_processes = MAX(1, MIN(n_processes, MAX_PARTS));
    /* The main process PID is stored. */
    sort->ppid = getpid();
    /* Delay for the algorithm in ns (less than 1s). */
    sort->delay = MAX(1, MIN(999999999, delay));

    if (!(file = fopen(file_name, "r"))) {
        perror("init_sort - fopen");
        return ERROR;
    }

    /* The first line contains the size of the data, truncated to MAX_DATA. */
    if (!(fgets(string, MAX_STRING, file))) {
        fprintf(stderr, "init_sort - Error reading file\n");
        fclose(file);
        return ERROR;
    }
    sort->n_elements = atoi(string);
    if (sort->n_elements > MAX_DATA) {
        sort->n_elements = MAX_DATA;
    }

    /* The remaining lines contains one integer number each. */
    for (i = 0; i < sort->n_elements; i++) {
        if (!(fgets(string, MAX_STRING, file))) {
            fprintf(stderr, "init_sort - Error reading file\n");
            fclose(file);
            return ERROR;
        }
        sort->data[i] = atoi(string);
    }
    fclose(file);

    /* Each task should have at least one element. */
    log_data = compute_log(sort->n_elements);
    if (n_levels > log_data) {
        n_levels = log_data;
    }
    sort->n_levels = n_levels;

    /* The data is divided between the tasks, which are also initialized. */
    block_size = sort->n_elements / get_number_parts(0, sort->n_levels);
    modulus = sort->n_elements % get_number_parts(0, sort->n_levels);
    sort->tasks[0][0].completed = INCOMPLETE;
    sort->tasks[0][0].ini = 0;
    sort->tasks[0][0].end = block_size + (modulus > 0);
    sort->tasks[0][0].mid = NO_MID;
    for (j = 1; j < get_number_parts(0, sort->n_levels); j++) {
        sort->tasks[0][j].completed = INCOMPLETE;
        sort->tasks[0][j].ini = sort->tasks[0][j - 1].end;
        sort->tasks[0][j].end = sort->tasks[0][j].ini \
            + block_size + (modulus > j);
        sort->tasks[0][j].mid = NO_MID;
    }
    for (i = 1; i < n_levels; i++) {
        for (j = 0; j < get_number_parts(i, sort->n_levels); j++) {
            sort->tasks[i][j].completed = INCOMPLETE;
            sort->tasks[i][j].ini = sort->tasks[i - 1][2 * j].ini;
            sort->tasks[i][j].mid = sort->tasks[i - 1][2 * j].end;
            sort->tasks[i][j].end = sort->tasks[i - 1][2 * j + 1].end;
        }
    }

    return OK;
}

Bool check_task_ready(Sort *sort, int level, int part) {
    if (!(sort)) {
        return FALSE;
    }

    if ((level < 0) || (level >= sort->n_levels) \
        || (part < 0) || (part >= get_number_parts(level, sort->n_levels))) {
        return FALSE;
    }

    if (sort->tasks[level][part].completed != INCOMPLETE) {
        return FALSE;
    }

    /* The tasks of the first level are always ready. */
    if (level == 0) {
        return TRUE;
    }

    /* Other tasks depend on the hierarchy. */
    if ((sort->tasks[level - 1][2 * part].completed == COMPLETED) && \
        (sort->tasks[level - 1][2 * part + 1].completed == COMPLETED)) {
        return TRUE;
    }

    return FALSE;
}

Status solve_task(Sort *sort, int level, int part) {
    /* In the first level, bubble-sort. */
    if (sort->tasks[level][part].mid == NO_MID) {
        return bubble_sort(\
            sort->data + sort->tasks[level][part].ini, \
            sort->tasks[level][part].end - sort->tasks[level][part].ini, \
            sort->delay);
    }
    /* In other levels, merge. */
    else {
        return merge(\
            sort->data + sort->tasks[level][part].ini, \
            sort->tasks[level][part].mid - sort->tasks[level][part].ini, \
            sort->tasks[level][part].end - sort->tasks[level][part].ini, \
            sort->delay);
    }
}

typedef struct {
	int level;
    int part;
    pid_t pid;
} Message;

int countSIGTERM, countSIGINT, countSIGALARM, fd[2], fd2[2];
Sort *sort = NULL;
Message receive;

void manejador_SIGALRM(int sig){

    /* Cierre del descriptor de entrada en el hijo */
    close(fd[0]) ;
    if( write(fd[1], &receive, MAX_CHAR) == -1){
        perror("write");
        exit(EXIT_FAILURE);
    }

    if(alarm(1)) {
        fprintf ( stderr , "Existe una alarma previa establecida \n") ;
    }
    countSIGALARM=1;
}

void manejador_SIGUSR1(int sig){}

void manejador_SIGINT(int sig){
    countSIGINT=1;
}

void manejador_SIGTERM(int sig){
    countSIGTERM=1;
}

Status sort_single_process(char *file_name, int n_levels, int n_processes, int delay) {
    int i, j, fd_shm, pipe_status, one, readd;
    pid_t *pid=NULL;
    Message msg;
    sigset_t set;
    struct sigaction act;
    mqd_t queue;
    sem_t *sem= NULL;
    ssize_t nbytes;

    struct mq_attr attributes;
    attributes.mq_flags = 0;
    attributes.mq_maxmsg = 10;
    attributes.mq_curmsgs = 0;
    attributes.mq_msgsize = sizeof(Message);

    countSIGTERM=0;

    /* Asignamos los manejadores para SIGUSR1 y SIGTERM*/
    sigemptyset(&(act.sa_mask));
    act.sa_flags = 0;

    act.sa_handler = manejador_SIGTERM;
    if (sigaction(SIGTERM, &act, NULL) < 0) {
        perror("sigaction");
        return ERROR;
    }

    act.sa_handler = manejador_SIGINT;
    if (sigaction(SIGINT, &act, NULL) < 0) {
        perror("sigaction");
        return ERROR;
    }

    act.sa_handler = manejador_SIGALRM;
    if ( sigaction(SIGALRM, &act, NULL ) < 0) {
        perror ("sigaction") ;
        exit ( EXIT_FAILURE ) ;
    }

    sigaddset(&(act.sa_mask), SIGUSR1); /* Si llega otra señal sigusr1 mientras esta en el manejador la bloquea*/
    act.sa_handler = manejador_SIGUSR1;
    if (sigaction(SIGUSR1, &act, NULL) < 0) {
        perror("sigaction");
        return ERROR; 
    }

    /* We create the shared memory */
    fd_shm = shm_open(SHM_NAME,
        O_RDWR | O_CREAT | O_EXCL, /* Create it and open for reading and writing */
        S_IRUSR | S_IWUSR); /* The current user can read and write */
    if (fd_shm == -1) {
        perror("shm_open");
        return ERROR; 
        }

    /* Resize the memory segment */
    if (ftruncate(fd_shm, sizeof(Sort)) == -1) {
        perror("ftruncate");
        shm_unlink(SHM_NAME);
        return ERROR; 
    }

    /* Map the memory segment */
    sort = mmap(NULL,
        sizeof(*sort), /* Memory mapping size */
        PROT_READ | PROT_WRITE, /* Read and write */
        MAP_SHARED, /* Share data changes */
        fd_shm, /* File or mem_file descriptor */
        0);
    close(fd_shm);
    if (sort == MAP_FAILED) {
        perror("mmap");
        shm_unlink(SHM_NAME);
        return ERROR; 
    }

    /* The data is loaded and the structure initialized. */
    if (init_sort(file_name, sort, n_levels, n_processes, delay) == ERROR) {
        perror("init_sort");
        shm_unlink(SHM_NAME);
        return ERROR;
    }

    queue = mq_open(MQ_NAME,
        O_CREAT | O_RDWR, /* This process is only going to send messages */
        S_IRUSR | S_IWUSR, /* The user can read and write */
        &attributes);

    if (queue == (mqd_t)-1) {
        perror("mq_open");
        shm_unlink(SHM_NAME);
        return EXIT_FAILURE;
    }

    if((sem = sem_open(SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED){
        perror ("sem_open") ;
        shm_unlink(SHM_NAME);
        mq_close(queue);
        mq_unlink(MQ_NAME);
        exit ( EXIT_FAILURE ) ;
    }

    /* Reservamos memoria para los pids de los trabajadores */
    pid = (pid_t*)malloc(sizeof(pid_t)*n_processes);
    if(!pid){
        perror("malloc");
        shm_unlink(SHM_NAME);
        mq_close(queue);
        mq_unlink(MQ_NAME);
        return EXIT_FAILURE;
    }
    
    for (i=0;i<=n_processes;i++){ /*as many pipes as processes are*/
        pipe_status = pipe(fd);
        if( pipe_status == -1){
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (i=0;i<=n_processes;i++){/*as many pipes as processes are (x2) one for write and another one for reading*/
        pipe_status = pipe(fd2);
        if( pipe_status == -1){
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }
    /*crear hijos*/
	for(i=0; i <= n_processes; i++){
        pid[i]=fork();
        if(pid[i]<0){
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if(!pid[i]) break;
        if(i==n_processes) break;
    }


    if(!pid[i] && i==n_processes){ /*ilustrador*/

        /* Ignoramos la señal SIGINT */
        if (signal(SIGINT, SIG_IGN)==SIG_ERR) {
            perror("signal");
            mq_close(queue);
            munmap(sort, sizeof(*sort));
            exit(EXIT_FAILURE);
        }

      /*  countSIGTERM = 0;
        countSIGINT = 0;
        sigfillset(&set);
        sigdelset(&set, SIGINT);
        sigsuspend(&set);*/
        
        if(countSIGINT == 1){
            free(pid);
            mq_close(queue);
            munmap(sort, sizeof(*sort));
            exit(EXIT_SUCCESS);
        }

        /* Cierre del descriptor de salida en el padre */
        close (fd[1]);
        nbytes = 0;
        do {
            nbytes = read(fd[0], &receive, MAX_CHAR);
            if( nbytes == -1){
                perror ("read");
                free(pid);
                mq_close(queue);
                munmap(sort, sizeof(*sort));
                exit ( EXIT_FAILURE ) ;
            }if( nbytes > 0){
                if(receive.level != -1 && receive.part != -1){
                    printf ("Proceso con pid %d trabajando en\tLevel: %i\tPart: %i\n", receive.pid, receive.level, receive.part);
                } else {
                    printf ("Proceso con pid %d no esta realizando ninguna tarea.\n", receive.pid);
                }
            }
        }while(nbytes!=0);
        
        /* Cierre del descriptor de entrada en el hijo */
        close(fd2[0]);
        one = 1;
        if( write(fd2[1], &one,  MAX_CHAR) == -1){
            perror("write2");
            exit(EXIT_FAILURE);
        }
        
        free(pid);
        mq_close(queue);
        munmap(sort, sizeof(*sort));
        exit(EXIT_SUCCESS);

    }else if(!pid[i]){ /*trabajadores(hijos)*/

        /* Ignoramos la señal SIGINT */
        if (signal(SIGINT, SIG_IGN)==SIG_ERR) {
            perror("signal");
            mq_close(queue);
            munmap(sort, sizeof(*sort));
            exit(EXIT_FAILURE);
        }

        if(alarm(1)) {
            fprintf ( stderr , "Existe una alarma previa establecida \n") ;
        }
        
        while(countSIGTERM==0){
            
            receive.level=-1;
            receive.part=-1;
            receive.pid = getpid();
            
            a: countSIGALARM=0;
            if (mq_receive(queue, (char*)&receive, sizeof(Message), NULL) == -1) {
                if(countSIGALARM==1)goto a;
                if(countSIGTERM==1) break;

                perror("mq_receive");
                mq_close(queue);
                munmap(sort, sizeof(*sort));
                exit(EXIT_FAILURE);
            }
            receive.pid = getpid();

            /*COMPROBRAR ERROR DE SOLVE_TASK???*/
            if(solve_task(sort, receive.level, receive.part)==ERROR){
                fprintf(stderr, "Error in solve_task.\n");
                mq_close(queue);
                munmap(sort, sizeof(*sort));
                exit(EXIT_FAILURE);
            }

            sem_wait(sem);  
            printf("\n%10s%10s%10s%10s%10s\n", "PID", "LEVEL", "PART", "INI", \
                "END");
            printf("%10d%10d%10d%10d%10d\n", getpid(), receive.level, receive.part, \
                sort->tasks[receive.level][receive.part].ini, sort->tasks[receive.level][receive.part].end);

            /* Ponemos la tarea como completada */
            sort->tasks[receive.level][receive.part].completed = COMPLETED;
            sem_post(sem);
               
            kill(getppid(), SIGUSR1);


            
            close (fd2[1]);
            do {
                nbytes = read(fd2[0], &readd,  MAX_CHAR);
                if( nbytes == -1){
                    perror ("read2") ;
                    free(pid);
                    mq_close(queue);
                    munmap(sort, sizeof(*sort));
                    exit ( EXIT_FAILURE ) ;
                }if( nbytes > 0){
                    if(readd == 1){
                        break;
                    } 
                }
            }while(nbytes!=0);
        }

        free(pid);
        mq_close(queue);
        munmap(sort, sizeof(*sort));
        exit(EXIT_SUCCESS);


    } else { /* proceso principal */
        countSIGINT=0;
        plot_vector(sort->data, sort->n_elements);
        printf("\nStarting algorithm with %d levels and %d processes...\n", sort->n_levels, sort->n_processes);

        /* For each level, and each part, the corresponding task is sent. */
        for (i = 0; i < sort->n_levels && countSIGINT==0 ; i++) {/*nivel*/
            msg.level=i;
            for (j = 0; j < get_number_parts(i, sort->n_levels) && countSIGINT==0 ; j++) {/*partes del nivel i*/
                msg.part=j;
                if (mq_send(queue, (char *)&msg, sizeof(Message), 1) == -1) {
                    perror("mq_send");
                    mq_close(queue);
                    mq_unlink(MQ_NAME);
                    munmap(sort, sizeof(*sort));
                    shm_unlink(SHM_NAME);
                    return ERROR;
                }
                if(countSIGINT==1) break;
                
            }

            /* Espera a que todas la tareas hayan sido realizadas */
            sigfillset(&set);
            sigdelset(&set, SIGUSR1);
            sigdelset(&set, SIGINT);
            j=0;
            while(j<get_number_parts(i, sort->n_levels) && countSIGINT==0){
                sigsuspend(&set);
                
                if(countSIGINT==1) break;

                sem_wait(sem);
                for (j = 0; j < get_number_parts(i, sort->n_levels); j++) {
                    if(sort->tasks[i][j].completed!=COMPLETED){break;}
                }
                sem_post(sem);
                
            }

            if(countSIGINT==1) break;

            printf("Nivel %d terminado.\n", i);

            plot_vector(sort->data, sort->n_elements);
        }

        if(countSIGINT==0){
            printf("Algoritmo completo.\n");
        } else {
            printf("Señal SIGINT recibida.\n");
        }
        
        printf("Finalizando hijos...\n");

        /* Enviamos SIGTERM a todos los trabajadores */
        for(i=0; i<n_processes ; i++){
            kill(pid[i], SIGTERM);
        }
        /*Esperamos a que terminen todos los hijos*/
        while(wait(NULL)!=-1);

        /*Liberamos recursos y */
        mq_close(queue);
        mq_unlink(MQ_NAME);
        munmap(sort, sizeof(*sort));
        shm_unlink(SHM_NAME);
        sem_close(sem);
        sem_unlink(SEM_NAME);
        return OK;
    }

}