#include "queue.h"

void consumer() {

    int array[MAXQUEUE];

    for (int i = 0; i < MAXQUEUE; i++)
        array[i] = 0;

    int fd_shm = shm_open(SHM_NAME, O_RDONLY, 0);
    if (fd_shm == -1) {
        fprintf(stderr, "Error opening the shared memory segment\n");
        return;
    }

    Queue *q = mmap(NULL, sizeof(*q), PROT_READ, MAP_SHARED, fd_shm, 0);
    close(fd_shm);
    if (q == MAP_FAILED) {
        fprintf(stderr, "Error mapping the shared memory segment \n");
        return;
    }

    sem_wait(q->sem_read);
    while(1) {
        sem_wait(q->sem_fill);
        sem_wait(q->sem_mutex);
        int print = q->items[q->front];
        if (print == -1) {
            sem_post(q->sem_mutex);
            sem_post(q->sem_empty);
            break;
        }
        array[print] += 1;
        sem_post(q->sem_mutex);
        sem_post(q->sem_empty);
    }
    
    for (int n = 0; n < MAXQUEUE; n++)
        printf("Number %d repeated %d times\n", n, array[n]);

    sem_post(q->sem_read);
    munmap(q, sizeof(*q));
    exit(EXIT_SUCCESS);
}

