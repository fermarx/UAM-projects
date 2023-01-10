#ifndef HTTP_H
#define HTTP_H

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <time.h>

int linea_peticion(int Cliente, char *meth, char *read_path, int minor_version, char *server_signature, char *buffer);

int linea_get(int Cliente, char *url, char *server_signature, int minor_version);

int linea_post(int Cliente, char *read_path, char *server_signature, int minor_version, char *buffer);

int linea_options(int Cliente, char *server_signature, int minor_version);

int send_file(int Cliente, char *path);

char *get_date();

char *get_content_type(char *read_path);

#endif