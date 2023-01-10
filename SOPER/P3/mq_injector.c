#include <fcntl.h>
#include <errno.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#define MSGSIZE 2000
#define MAXMSG 10

typedef struct {
    char aviso[MSGSIZE];
} Mensaje;

int main(int argc, char **argv){

    FILE *pf = NULL;
    Mensaje msg[MAXMSG];
    mqd_t queue;
    int i;

    struct mq_attr attributes = {
        .mq_flags = 0,
        .mq_maxmsg = MAXMSG,
        .mq_curmsgs = 0,
        .mq_msgsize = sizeof(Mensaje)
    };

    if(argc < 3){
        printf("Enter ./<Executable> <text.txt> </queue_name>\n");
        exit(EXIT_FAILURE);
    }

    queue = mq_open(argv[2],
        O_CREAT | O_WRONLY, /* This process is only going to send messages */
        S_IRUSR | S_IWUSR, /* The user can read and write */
        &attributes);

    pf = fopen(argv[1], "r");
    if(!pf){
        perror("fopen: ");
        exit(EXIT_FAILURE);
    }


    for(i=0; fgets(msg[i].aviso, MSGSIZE, pf) != NULL; i++){
        if (mq_send(queue, (char*)&msg[i], sizeof(msg[i]), 1) == -1) {
            perror("Send message: ");
            fclose(pf);
            exit(EXIT_FAILURE);
        }
    }
    fclose(pf);
    
    printf("Mensages insertados correctamente.\n");

    fprintf(stdout, "Inserta cualquier tecla para finalizar.\n");
    getchar();

    mq_close(queue);
    mq_unlink(argv[2]);

    exit(EXIT_SUCCESS);
}