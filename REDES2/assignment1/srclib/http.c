#include "../includes/http.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int linea_peticion(int Cliente, char *meth, char *read_path, int minor_version, char *server_signature, char *buffer)
{
    /* Comprobamos que la version es compatible*/
    if (minor_version != 1)
    {
        //error_message(505, Cliente);
        printf("Error 505\n");
        return -1;
    }

    if (strcmp(meth, "GET") == 0)
    {
        linea_get(Cliente, read_path, server_signature, minor_version);
    }
    else if (strcmp(meth, "POST") == 0)
    {
        linea_post(Cliente, read_path, server_signature, minor_version, buffer);
    }
    else if (strcmp(meth, "OPTIONS") == 0)
    {
        linea_options(Cliente, server_signature, minor_version);
    }
    /*
    else
    {
        error_message(501, Cliente);
    }
    */
    return 0;
}

int linea_get(int Cliente, char *read_path, char *server_signature, int minor_version)
{
    char *date, *type;
    char response[1024];
    struct stat file;

    /* Comprobar que el archivo exista, si no existe enviar 404 */
    if (access(read_path, F_OK) == 0)
    {

        printf("URL: %s\n", read_path);

        date = get_date();
        type = get_content_type(read_path);
        stat(read_path, &file);

        sprintf(response, "HTTP/1.%d 200 OK\r\n", minor_version);
        send(Cliente, response, strlen(response), 0);
        sprintf(response, "Date: %s\r\n", date);
        send(Cliente, response, strlen(response), 0);
        sprintf(response, "Server: %s\r\n", server_signature);
        send(Cliente, response, strlen(response), 0);
        sprintf(response, "Last-Modified: %s", ctime(&file.st_mtime));
        send(Cliente, response, strlen(response), 0);
        sprintf(response, "Content-Length: %d\r\n", (int)file.st_size);
        send(Cliente, response, strlen(response), 0);
        sprintf(response, "Content-Type: %s\r\n", type);
        send(Cliente, response, strlen(response), 0);
        sprintf(response, "\r\n");
        send(Cliente, response, strlen(response), 0);

        //printf("Content-type: %s", type);

        /*Send requested resource*/
        send_file(Cliente, read_path);
        free(date);
        free(type);
    }
    else
    {
        printf("El recurso solicitado no existe.\n");
    }

    return 0;
}

int linea_post(int Cliente, char *read_path, char *server_signature, int minor_version, char *buffer)
{
    char *date, *type;
    char response[1024];
    struct stat file;

    /* Comprobar que el archivo exista, si no existe enviar 404 */
    if (access(read_path, F_OK) == 0)
    {

        printf("URL: %s\n", read_path);

        date = get_date();
        type = get_content_type(read_path);
        stat(read_path, &file);

        sprintf(response, "HTTP/1.%d 200 OK\r\n", minor_version);
        send(Cliente, response, strlen(response), 0);
        sprintf(response, "Date: %s\r\n", date);
        send(Cliente, response, strlen(response), 0);
        sprintf(response, "Server: %s\r\n", server_signature);
        send(Cliente, response, strlen(response), 0);
        sprintf(response, "Last-Modified: %s", ctime(&file.st_mtime));
        send(Cliente, response, strlen(response), 0);
        sprintf(response, "Content-Length: %d\r\n", (int)file.st_size);
        send(Cliente, response, strlen(response), 0);
        sprintf(response, "Content-Type: %s\r\n", type);
        send(Cliente, response, strlen(response), 0);
        sprintf(response, "\r\n");
        send(Cliente, response, strlen(response), 0);

        //printf("Content-type: %s", type);

        /*Send requested resource*/
        send_file(Cliente, read_path);
        free(date);
        free(type);
    }
    else
    {
        printf("El recurso solicitado no existe.\n");
    }

    return 0;
}

int linea_options(int Cliente, char *server_signature, int minor_version)
{
    char response[1024];
    char *date;

    date = get_date();

    sprintf(response, "HTTP/1.%d 200 OK\r\n", minor_version);
    send(Cliente, response, strlen(response), 0);
    sprintf(response, "Date: %s\r\n", date);
    send(Cliente, response, strlen(response), 0);
    sprintf(response, "Allow: GET, POST, OPTIONS\r\n");
    send(Cliente, response, strlen(response), 0);
    sprintf(response, "Server: %s\r\n", server_signature);
    send(Cliente, response, strlen(response), 0);
    sprintf(response, "Content-Length: 0\r\n");
    send(Cliente, response, strlen(response), 0);
    sprintf(response, "Content-Type: text/plain\r\n");
    send(Cliente, response, strlen(response), 0);
    sprintf(response, "\r\n");
    send(Cliente, response, strlen(response), 0);

    free(date);
    return 0;
}

int send_file(int Cliente, char *path)
{
    int len;
    char buf[1024];
    FILE *f = NULL;

    f = fopen(path, "rb");
    if (!f)
    {
        printf("Error en send_file\n");
        return -1;
    }

    while (!feof(f))
    {
        len = fread(buf, sizeof(char), 1024, f);

        if (len < 0)
        {
            printf("Error en send_file\n");
            fclose(f);
            return -1;
        }
        else if (len > 0)
        {
            if (send(Cliente, buf, len, 0) == -1)
            {
                printf("Error en send\n");
                return -1;
            }
        }
    }

    fclose(f);
    return 0;
}

char *get_date()
{
    char *date;
    date = (char *)malloc(sizeof(char) * 128);
    if (!date)
    {
        printf("Error en http response\n");
        return NULL;
    }
    time_t current_time = time(NULL);
    struct tm *t = gmtime(&current_time);
    strftime(date, 128, "%a, %d %b %Y %H:%M:%S %Z", t);
    return date;
}

char *get_content_type(char *read_path)
{
    char *type;
    type = (char *)malloc(sizeof(char) * 64);
    char *str_type;

    str_type = strchr(read_path, '.');

    if (strcmp(str_type, ".txt") == 0)
        strcpy(type, "text/plain");
    else if (strcmp(str_type, ".htm") == 0 || strcmp(str_type, ".html") == 0)
        strcpy(type, "text/html");
    else if (strcmp(str_type, ".gif") == 0)
        strcpy(type, "image/gif");
    else if (strcmp(str_type, ".jpeg") == 0 || strcmp(str_type, ".jpg") == 0)
        strcpy(type, "image/jpeg");
    else if (strcmp(str_type, ".mpg") == 0 || strcmp(str_type, ".mpeg") == 0)
        strcpy(type, "video/mpeg");
    else if (strcmp(str_type, ".doc") == 0 || strcmp(str_type, ".docx") == 0)
        strcpy(type, "application/msword");
    else if (strcmp(str_type, ".pdf") == 0)
        strcpy(type, "application/pdf");
    else if (strcmp(str_type, ".ico") == 0)
        strcpy(type, "image/x-icon");

    return type;
}