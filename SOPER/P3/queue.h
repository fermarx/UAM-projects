#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#define MAXQUEUE 10
#define SHM_NAME "/shm_1"
#define SEM1_NAME "/empty"
#define SEM2_NAME "/fill"
#define SEM3_NAME "/mutex"
#define SEM4_NAME "/read"

typedef struct {
    int items[MAXQUEUE];       
    int front;    
    sem_t *sem_read, *sem_empty, *sem_mutex, *sem_fill;         
} Queue;

void consumer();

#endif