#pragma once

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <pthread.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "libmx.h"
#include "client.h"
#include "server.h"
#include "json.h"

// Constants
#define MX_SUCCESS 0
#define MX_FAILURE 1
#define MX_SSL_FAILURE -1

#define MX_SA struct sockaddr
#define MX_MAX 10500

#define MX_CAESAR_KEY 5
#define MX_VIGENERE_KEY "UCHAT"

// Structures


// Functions
//Utils
bool mx_check_port(char *port);
bool mx_check_ip(char *ip);
void mx_send_file(int socket_fd, char *path);
void mx_recv_file(int connection_fd, char *filename);

// Encryption (Vigenere + Caesar ciphers combination)
char *mx_generate_key(char *str, char *key);
char *mx_encrypt(char *str, char *key);
char *mx_decrypt(char *str, char *key);

