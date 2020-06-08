#pragma once

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
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

// Structures


// Functions
//Utils
bool mx_check_port(char *port);
bool mx_check_ip(char *ip);
