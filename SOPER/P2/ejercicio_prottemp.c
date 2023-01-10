#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

int countSenales=0;


void manejador_SIGALRM(int sig) {
    
}

void manejador_SIGTERM(int sig) {
    printf("Finalizado %d\n", getpid());
    exit(EXIT_SUCCESS);
}

void manejador_SIGUSR2(int sig) {
    countSenales++;
}



int main(int argc, char **argv){

    pid_t *pid;
    int N, T, i = 0, count = 0, pid_actual;
    sigset_t set, set2;
    struct sigaction act;


    if (argc < 3){
        printf ("./<executable> <n_hijos> <tiempo_suspension>\n");
        exit(EXIT_FAILURE);
    }

    N = atoi(argv[1]);
    T = atoi(argv[2]);


    /* Creas el manejador de SIGUSR2 y SIGTERM antes de hacer
     * el fork para asegurarte de que los manejadores ya están
     * asignados a los hijos y al padre antes de que se envíe
     * alguna señal.
     */
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

    /* Creas memoria para guardar tantos PIDs como hijos haya */
    pid = (pid_t*)malloc(sizeof(pid_t)*N);
    if(!pid){
        perror("Malloc: ");
        exit(EXIT_FAILURE);
    }

    /* Guardas los PIDs de los hijos en la memoria anteriormente creada */
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

    if(!pid[i]){    /*hijo*/
        
        /* Liberamos el recurso pid que el hijo no va a utilizar */
        free(pid);

        pid_actual=getpid();

        /* Operación pedida en el enunciado */
        for(i=1; i<pid_actual/10; i++){
            count+= i; 
        }
        
        printf("PID del hijo: %i. Total: %i\n", getpid(), count);
            
        /* El hojo manda la señal SIGUSR2 al padre */
        kill(getppid(), SIGUSR2);

        /* Se rellena la máscara set con todas las señales y se elimina
         * la señal SIGTERM, ya que más adelante bloquearemos todas las
         * señales que reciba, excepto esa.
         */
        sigfillset(&set);
        sigdelset(&set, SIGTERM);

        sigsuspend(&set);
        

    } else {    /*padre*/
        
        /**
        * Asignamos el manejador de SIGALRM solo para
        * el proceso padre ya que no es necesario
        * para los hijos.  
        */
        sigemptyset(&(act.sa_mask));
        act.sa_flags = 0;

        act.sa_handler = manejador_SIGALRM;
        if (sigaction(SIGALRM, &act, NULL) < 0) {
            perror("sigaction");
            exit(EXIT_FAILURE); 
        }

        /**
        * Asignamos una alarma de tiempo T 
        */
        if (alarm(T)) {
            fprintf(stderr, "Alarm.\n");
        }

        /**
        * Utilizamos sigsuspend con una máscara con todas
        * las señales excepto SIGALRM para que el proceso
        * solo espere por esa.
        */ 
        sigfillset(&set);
        sigdelset(&set, SIGALRM);

        sigsuspend(&set);


        /* Mandas a el hijo (con PID guardado en la memoria 
         * anteriormente creada) la señal SIGTERM.
         */
        for(i=0; i<N; i++){
            kill(pid[i], SIGTERM);
        }

        printf("\nFinalizado Padre.\n");
        printf("Número de señales SIGUSR2 recibidas: %d\n", countSenales);

        free(pid);

        wait(NULL);

    }

    exit(EXIT_SUCCESS);
}