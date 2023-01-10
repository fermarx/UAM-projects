#include "../includes/methods.h"
#include <strings.h>

int mySocket(int domain, int type, int protocol)
{

    int Descriptor;

    Descriptor = socket(domain, type, protocol);
    if (Descriptor < 0)
    {
        printf("Error en mySocket\n");
        syslog(LOG_ERR, "Error en mySocket: %d", errno);
        return -1;
    }

    return Descriptor;
}

struct sockaddr_in myAddress(int puerto)
{
    struct sockaddr_in Direccion;

    Direccion.sin_family = AF_INET;
    Direccion.sin_port = htons(puerto);
    Direccion.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero((void *)&(Direccion.sin_zero), 8);

    return Direccion;
}

struct sockaddr_in getAddress()
{
    FILE *config;
    char str[128];
    int listen_port;

    config = fopen("server.conf", "r");
    if (config == NULL)
    {
        printf("Error abriendo archivo de configuración\n");
        syslog(LOG_ERR, "Error abriendo archivo de configuración: %d", errno);
    }

    while (fgets(str, 128, config))
    {
        if (strncmp("listen_port", str, strlen("listen_port")) == 0)
        {
            strtok(str, " \n");
            strtok(NULL, " \n");
            listen_port = atoi(strtok(NULL, " \n"));
        }
    }
    fclose(config);
    return myAddress(listen_port);
}

bool myBind(int Descriptor, struct sockaddr_in Direccion)
{

    if (bind(Descriptor, (struct sockaddr *)&Direccion, sizeof(Direccion)) == -1)
    {
        syslog(LOG_ERR, "Error en myBind: %d", errno);
        return false;
    }

    return true;
}

int myAccept(int Descriptor)
{
    socklen_t len;
    int value;
    struct sockaddr direccion;

    len = sizeof(direccion);

    value = accept(Descriptor, &direccion, &len);
    if (value < 0)
    {
        syslog(LOG_ERR, "Error en myAccept: %d", errno);
        return -1;
    }
    printf("Conexion establecida\n");
    return value;
}

int myListen(int Descriptor, int max_clients)
{
    int value;
    value = listen(Descriptor, max_clients);
    if (value < 0)
    {
        printf("Error en myListen\n");
        syslog(LOG_ERR, "Error en myListen: %d", errno);
        return -1;
    }
    return value;
}

int iniciar_servidor(FILE *config)
{
    char str[128];
    int listen_port, max_clients;
    int Descriptor;
    struct sockaddr_in Direccion;

    /* Leemos nº de clientes máximo y puerto del archivo de configuración */
    while (fgets(str, 128, config))
    {
        if (strncmp("max_clients", str, strlen("max_clients")) == 0)
        {
            strtok(str, " \n");
            strtok(NULL, " \n");
            max_clients = atoi(strtok(NULL, " \n"));
        }
        else if (strncmp("listen_port", str, strlen("listen_port")) == 0)
        {
            strtok(str, " \n");
            strtok(NULL, " \n");
            listen_port = atoi(strtok(NULL, " \n"));
        }
    }
    printf("max_clients: %d listen_port: %d\n", max_clients, listen_port);
    fclose(config);

    Descriptor = mySocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (Descriptor == -1)
    {
        printf("Error abriendo socket\n");
        return -1;
    }

    Direccion = myAddress(listen_port);

    if (myBind(Descriptor, Direccion) == false)
    {
        printf("Error en myBind\n");
        return -1;
    }

    if (myListen(Descriptor, max_clients) == -1)
    {
        return -1;
    }
    printf("Servidor a la escucha en el puerto %d\n", ntohs(Direccion.sin_port));

    return Descriptor;
}

char *get_server_route()
{
    FILE *config;
    char str[128];
    char *server_route;

    server_route = (char *)malloc(128 * sizeof(char));

    config = fopen("server.conf", "r");
    if (config == NULL)
    {
        printf("Error abriendo archivo de configuración\n");
        syslog(LOG_ERR, "Error abriendo archivo de configuración: %d", errno);
    }

    while (fgets(str, 128, config))
    {
        if (strncmp("server_route", str, strlen("server_route")) == 0)
        {
            strtok(str, " \n");
            strtok(NULL, " \n");
            sprintf(server_route, "%s", strtok(NULL, " \n"));
        }
    }
    fclose(config);
    return server_route;
}

char *get_server_signature()
{
    FILE *config;
    char str[128];
    char *server_signature;

    server_signature = (char *)malloc(128 * sizeof(char));

    config = fopen("server.conf", "r");
    if (config == NULL)
    {
        printf("Error abriendo archivo de configuración\n");
        syslog(LOG_ERR, "Error abriendo archivo de configuración: %d", errno);
    }

    while (fgets(str, 128, config))
    {
        if (strncmp("server_signature", str, strlen("server_signature")) == 0)
        {
            strtok(str, "=");
            sprintf(server_signature, "%s", strtok(NULL, "\n"));
        }
    }
    fclose(config);
    return server_signature;
}

void demonizar()
{
    char directorio[64];
    pid_t pid;

    getcwd(directorio, sizeof(directorio));

    pid = fork();
    if (pid < 0)
    {
        //Error en fork
        printf("Error en fork\n");
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
    {
        /* Terminamos proceso padre */
        exit(EXIT_SUCCESS);
    }

    umask(0);
    if (setsid() < 0)
    {
        printf("Error al crear SIS\n");
        exit(EXIT_FAILURE);
    }
    if ((chdir("/")) < 0)
    {
        syslog(LOG_ERR, "Error cambiando de directorio");
        exit(EXIT_FAILURE);
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    return;
}
