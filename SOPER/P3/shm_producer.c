#include "queue.h"

int main(int argc, char **argv) {
    
    if (argc < 3) {
        printf("Enter <./Executable> <numbers_to_generate> <0: random numbers || 1: secuency numbers>\n");
        exit(EXIT_FAILURE);
    }

    int N = atoi(argv[1]);
    int R = atoi(argv[2]);
    if (R != 0 && R != 1){
        printf("Enter 0 for random numbers or 1 for secuency numbers\n");
        exit(EXIT_FAILURE);
    }

    int fd_shm = shm_open(SHM_NAME, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd_shm == -1) {
        fprintf(stderr, "Error creating the shared memory segment\n");
        return EXIT_FAILURE;
    }

    if (ftruncate(fd_shm, sizeof(Queue)) == -1) {
        fprintf(stderr, "Error resizing the shared memory segment\n");
        shm_unlink(SHM_NAME);
        return EXIT_FAILURE;
    }

    Queue *q = mmap(NULL, sizeof(*q),PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
    close(fd_shm); 
    if (q == MAP_FAILED) {
        fprintf(stderr, "Error mapping the shared memory segment\n");
        shm_unlink(SHM_NAME);
        return EXIT_FAILURE;
    }

    if ((q->sem_empty = sem_open(SEM1_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED) {
		perror("sem_open");
        shm_unlink(SHM_NAME);
		exit(EXIT_FAILURE);
	}

    if ((q->sem_mutex = sem_open(SEM2_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED) {
		perror("sem_open");
        shm_unlink(SHM_NAME);
		exit(EXIT_FAILURE);
	}

    if ((q->sem_fill = sem_open(SEM3_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED) {
		perror("sem_open");
        shm_unlink(SHM_NAME);
		exit(EXIT_FAILURE);
	}

    if ((q->sem_read = sem_open(SEM4_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED) {
		perror("sem_open");
        shm_unlink(SHM_NAME);
		exit(EXIT_FAILURE);
	}

    for(int i = 0; i < 9; i++) 
        q->items[i] = 0;

    pid_t pid = fork();
    if (pid == 0) {
        consumer();
    }
    else{
        sem_post(q->sem_read);

        if (R == 0){ 
            for(int i = 0; i < N; i++){
                sem_wait(q->sem_empty);
                sem_wait(q->sem_mutex);
                q->front = (q->front + 1)%MAXQUEUE;
                int random = rand() % 10;
                q->items[q->front] = random;
                if(i == N - 1) q->items[q->front] = -1;
                sem_post(q->sem_mutex);
                sem_post(q->sem_fill);
            }
        }
        if(R == 1){
            for(int i = 0; i < N; i++){
                sem_wait(q->sem_empty);
                sem_wait(q->sem_mutex);
                q->front = (q->front + 1)%MAXQUEUE;
                q->items[q->front] = i%10;
                if(i == N - 1) q->items[q->front] = -1;
                sem_post(q->sem_mutex);
                sem_post(q->sem_fill);
            

            }
        }
    }

    sem_wait(q->sem_read);
    sem_close(q->sem_read);
    sem_close(q->sem_empty);
    sem_close(q->sem_fill);
    sem_close(q->sem_mutex);
    munmap(q, sizeof(*q));
    sem_unlink(SEM1_NAME);
    sem_unlink(SEM2_NAME);
    sem_unlink(SEM3_NAME);
    sem_unlink(SEM4_NAME);
    shm_unlink(SHM_NAME);  

    return EXIT_SUCCESS; 
}

