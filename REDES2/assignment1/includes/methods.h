#ifndef METHODS_H
#define METHODS_H

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>
#include <netdb.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>

int mySocket(int domain, int type, int protocol);

struct sockaddr_in myAddress(int puerto);

struct sockaddr_in getAddress();

bool myBind(int Descriptor, struct sockaddr_in Direccion);

int myAccept(int Descriptor);

int myListen(int Descriptor, int max_clients);

int iniciar_servidor(FILE *config);

char *get_server_route();

char *get_server_signature();

void demonizar();

#endif