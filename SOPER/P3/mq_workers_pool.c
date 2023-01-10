#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

#define MSGSIZE 2000
#define FINALIZACION "fin"
#define MAXMSG 10

typedef struct {
    char aviso[MSGSIZE];
} Mensaje;

pid_t *pid;
int countMensajes, countCaracter;

void manejador_SIGTERM(int sig){
    printf("Mensajes procesados por %i = %i. Caracteres contados = %i\n", getpid(), countMensajes, countCaracter);
    exit(EXIT_SUCCESS);    
}

void manejador_SIGUSR2(int sig) {
    
    int i, N;
    
    if(!pid){return;}

    N = sizeof(pid)/sizeof(pid_t);

    for(i=0; i<N ; i++){
        kill(pid[i], SIGTERM);
    }

    free(pid);    
}


int main(int argc, char **argv){
    
    struct mq_attr attributes = {
        .mq_flags = 0,
        .mq_maxmsg = MAXMSG,
        .mq_curmsgs = 0,
        .mq_msgsize = MSGSIZE
    };

    int N, i, j;
    char search_char;
    Mensaje msg[MAXMSG];
    sigset_t set;
    struct sigaction act;

    if(argc<4){
        printf("Inserta ./<ejecutable> <numero de trabajadores (1-10)> <nombre de la cola de mensajes> <caracter a contar>\n");
        exit(EXIT_FAILURE);
    }

    N = atoi(argv[1]);
    search_char= argv[3][0];

    pid = (pid_t*)malloc(sizeof(pid_t)*N);
    if(!pid){
        perror("Malloc: ");
        exit(EXIT_FAILURE);
    }
    
    /*asignamos manejador a SIGUSR2*/
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

    mqd_t queue = mq_open(argv[2],
        O_CREAT | O_RDONLY | O_NONBLOCK, 
        S_IRUSR | S_IWUSR,
        &attributes);

    if(queue == (mqd_t)-1) {
        fprintf(stderr, "Error opening the queue\n");
        return EXIT_FAILURE;
    }

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
        countMensajes = 0;
        countCaracter = 0;

        for(i=0; i<MAXMSG; i++){

            if (mq_receive(queue, (char*)&msg[i], sizeof(msg[i]), NULL) == -1) {
                mq_close(queue);
                break;
            }

            if(strcmp(msg[i].aviso, FINALIZACION) == 0){ /*si lo que lee es igual a el mensaje de finalizacion termina*/
                mq_close(queue);
                break;
            }

            for(j=0; j<strlen(msg[i].aviso)-1; j++){  
                if(msg[i].aviso[j] == search_char){
                    countCaracter++;
                }               
            }

            countMensajes++;
        }
        kill(getppid(), SIGUSR2);
        sigfillset(&set);
        sigdelset(&set, SIGTERM);

        sigsuspend(&set);

    }else{ /*padre*/

        
        sigfillset(&set);
        sigdelset(&set, SIGUSR2);
        sigdelset(&set, SIGTSTP);

        sigsuspend(&set);

        mq_close(queue);
    } 

    exit(EXIT_SUCCESS);
}