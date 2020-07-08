#pragma once

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include <pthread.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>

#include "libmx.h"
#include "json.h"

// Constants
#define MX_SUCCESS 0
#define MX_FAILURE 1
#define MX_SSL_FAILURE -1

#define MX_SA struct sockaddr
#define MX_MAX 10500


// Structures
enum e_types {
    REG,
    S_IN,
    S_OUT,
    N_GRP,
    INV,
    RN_GR,
    LEAVE_GROUP,
    S_MES,
    EDIT_MESSAGE,
    DEL_USER_PROFILE,
    DELETE_MESSAGE,
    LOAD_DIALOGUES,
    LOAD_GROUPS,
    LOAD_MESSAGES,
    DELETE_USER,
    GET_USER
};

// Functions
//Utils
bool mx_check_port(char *port);
bool mx_check_ip(char *ip);
void mx_send_file(int socket_fd, char *path);
void mx_recv_file(int connection_fd, char *path);
