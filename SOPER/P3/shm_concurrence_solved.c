#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define SHM_NAME "/shm_eje3"
#define MAX_MSG 2000

static void getMilClock(char *buf) {
    int millisec;
	char aux[100];
    struct tm* tm_info;
    struct timeval tv;

    gettimeofday(&tv, NULL);
	millisec = lrint(tv.tv_usec/1000.0); // Round to nearest millisec
    if (millisec>=1000) { // Allow for rounding up to nearest second
        millisec -=1000;
        tv.tv_sec++;
    }
    tm_info = localtime(&tv.tv_sec);
    strftime(aux, 10, "%H:%M:%S", tm_info);
	sprintf(buf, "%s.%03d", aux, millisec);
}

typedef struct {
	pid_t processid;       /* Logger process PID */
	long logid;            /* Id of current log line */
	char logtext[MAX_MSG]; /* Log text */
	sem_t sem_write;
	sem_t sem_read;
} ClientLog;

ClientLog *shm_struct;
int countSigusr=0;

void manejador (int sig) {
	if (sig == SIGUSR1) {
		sem_wait(&(shm_struct->sem_write));
		printf ("Log %ld: Pid %d: %s\n",shm_struct->logid, shm_struct->processid, shm_struct->logtext);
		countSigusr++;
		sem_post(&(shm_struct->sem_read));
	}
}

int main(int argc, char *argv[]) {
	int n, m, i, pid, j;
	int ret = EXIT_FAILURE;
	sigset_t set;
	struct sigaction act;

	if (argc < 3) {
		fprintf(stderr,"usage: %s <n_process> <n_logs> \n",argv[0]);
		return ret;
	}

	n = atoi(argv[1]);
	m = atoi(argv[2]);
	
	int fd_shm = shm_open(SHM_NAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd_shm == -1) {
		perror("shm_open: ");
        fprintf(stderr, "Error opening the shared memory segment\n");
        return EXIT_FAILURE;
    }

	if (ftruncate(fd_shm, sizeof(ClientLog)) == -1) {
		perror("ftruncate");
        fprintf(stderr, "Error resizing the shared memory segment\n");
        shm_unlink(SHM_NAME);
        return EXIT_FAILURE;
    }

    shm_struct = mmap(NULL, sizeof(*shm_struct), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
	if(!shm_struct){
		perror("mmap: ");
		return ret;
	}
    shm_struct->logid = -1;
	close(fd_shm);
	
	if (shm_struct == MAP_FAILED) {
        fprintf(stderr, "Error mapping the shared memory segment\n");
        shm_unlink(SHM_NAME);
        return EXIT_FAILURE;
    }

	/*crear semaforo de la estructura*/
	if(sem_init(&(shm_struct->sem_write), 1, 1)==-1){
        perror("sem_init: ");
		return ret;
    }

    /*crear semaforo lectura*/
	if(sem_init(&(shm_struct->sem_read), 1, 1)==-1){
        perror("sem_init: ");
		return ret;
    }

    /*asignar manejador*/
	sigemptyset(&(act.sa_mask));
	sigaddset(&(act.sa_mask), SIGUSR1);
	act.sa_flags = 0;
	act.sa_handler =  manejador;
	if(sigaction(SIGUSR1, &act, NULL) < 0){
		perror("Sigaction");
		exit(EXIT_FAILURE);
	}
	sem_wait(&shm_struct->sem_write);

	/*crear hijos*/
	for(i=0; i < n; i++){
        pid=fork();
        if(pid<0){
            perror("Fork: ");
            exit(EXIT_FAILURE);
        }
        if(!pid){ break; }
		
    }

    if(!pid){/*hijo*/

		for(i = 0; i < m; i++){

			double t = 0;
			t = rand() % (100 - 900 + 1) + 900; //(N-M+1) + M rand num btw 100 and 900
			t = t/1000;
			sleep(t);

			sem_wait(&shm_struct->sem_read);

			shm_struct->processid = getpid();
			shm_struct->logid++;
			getMilClock(shm_struct->logtext);
			

			kill(getppid(), SIGUSR1);
			sem_post(&(shm_struct->sem_write));
		}

    }else{/*padre*/

		sigfillset(&set);
		sigdelset(&set, SIGUSR1);
		sigdelset(&set, SIGTSTP);

		while(countSigusr < (n*m)){
			
			sigsuspend(&set);
		}

		if(sem_destroy(&(shm_struct->sem_write))==-1){
			perror("sem_destroy: ");
			return ret;
		}
		if(sem_destroy(&(shm_struct->sem_read))==-1){
			perror("sem_destroy: ");
			return ret;
		}

		munmap(shm_struct, sizeof(*shm_struct));
		shm_unlink(SHM_NAME);

		
    }
}