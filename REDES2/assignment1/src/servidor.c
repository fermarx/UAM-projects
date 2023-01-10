#include "../includes/methods.h"
#include "../includes/picohttpparser.h"
#include "../includes/http.h"
#include <pthread.h>
#include <signal.h>

/* Variables globales para usar en el manejador de SIGINT */
int Descriptor, Cliente;

void handle_sigint(int sig)
{
    printf("\nTerminando procesos y cerrando sockets...\n");
    close(Descriptor);
    close(Cliente);
}

int parsear(int Cliente)
{
    char buf[1024], meth[16], read_path[128];
    char *server_route, *server_signature;
    const char *method, *path;
    int pret, minor_version;
    struct phr_header headers[100];
    size_t buflen = 0, prevbuflen = 0, method_len, path_len, num_headers;
    ssize_t rret;

    while (1)
    {
        /* Code taken from example in https://github.com/h2o/picohttpparser */
        while ((rret = read(Cliente, buf + buflen, sizeof(buf) - buflen)) == -1 && errno == EINTR)
            ;
        if (rret <= 0)
        {
            return -1;
        }

        prevbuflen = buflen;
        buflen += rret;
        /* parse the request */
        num_headers = sizeof(headers) / sizeof(headers[0]);

        pret = phr_parse_request(buf, buflen, &method, &method_len, &path, &path_len,
                                 &minor_version, headers, &num_headers, prevbuflen);
        if (pret > 0)
        {
            printf("successfully parsed the request\n");
            break;
        }
        else if (pret == -1)
        {
            printf("Error en pret\n");
        }

        if (buflen == sizeof(buf))
        {
            printf("Error en buffer\n");
            return -1;
        }
    }

    sprintf(meth, "%.*s", (int)method_len, method);
    server_route = get_server_route();
    server_signature = get_server_signature();
    printf("Method: %s\n", meth);
    printf("server_route: %s\n", server_route);
    printf("server_signature: %s\n", server_signature);

    if (path_len > 1)
    {
        sprintf(read_path, "%s%.*s", server_route, (int)path_len, path);
    }
    else
    {
        sprintf(read_path, "%s/index.html", server_route);
    }

    if (linea_peticion(Cliente, meth, read_path, minor_version, server_signature, buf) == -1)
    {
        printf("Error en linea peticion\n");
        return -1;
    }
    free(server_route);
    free(server_signature);

    return pret;
}

int proceso()
{
    pid_t pid;

    /*Las nuevas peticiones se ejecutarán en nuevos procesos que se generarán
      dinámicamente en vez de en un pool */
    while (1)
    {
        Cliente = myAccept(Descriptor);
        if (Cliente == -1)
        {
            continue;
        }
        /* En el nuevo proceso hijo */
        if ((pid = fork()) == 0)
        {
            if (parsear(Cliente) == -1)
            {
                printf("Error en parsear");
                return -1;
            }
            exit(EXIT_SUCCESS);
        }

        close(Cliente);
    }
    close(Descriptor);
    return 0;
}

int main(int argc, char **argv)
{
    FILE *config;
    struct sigaction act;

    /* Iniciando manejador de SIGINT */
    act.sa_handler = handle_sigint;
    sigemptyset(&(act.sa_mask));
    act.sa_flags = SIGINT;

    if (sigaction(SIGINT, &act, NULL) < 0)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    /* Abriendo archivo configuración del servidor */
    config = fopen("server.conf", "r");
    if (config == NULL)
    {
        printf("Error abriendo archivo de configuración\n");
        syslog(LOG_ERR, "Error abriendo archivo de configuración: %d", errno);
        return -1;
    }

    //demonizar();

    Descriptor = iniciar_servidor(config);
    if (Descriptor == -1)
    {
        return -1;
    }

    if (proceso() == -1)
    {
        return -1;
    }

    return EXIT_SUCCESS;
}