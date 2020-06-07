#pragma once

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/socket.h>
#include <sys/types.h>

#include "libmx.h"
#include "client.h"
#include "server.h"

// Constants
#define MX_SA struct sockaddr
#define MX_MAX 80

// RSA
#define MX_FIRST_PRIME 1
#define MX_SECOND_PRIME 3
#define MX_N(x, y) ((x) * (y))
#define MX_T(x, y) (((x) - 1) * ((y) - 1))

// Structures


// Functions
//Utils
bool mx_check_port(char *port);
bool mx_check_ip(char *ip);

// RSA
void mx_encryption_key(long int *e, long int *d);
char *mx_encrypt(char *data, long int *e, long int *m, long int *en);
char *mx_decrypt(long int *d, long int *m, long int *en);
