#include "uchat.h"

// {"type": "REG", "name": "kali", "passw": "qwerty"}
// {"type": "REG", "name": "geralt", "passw": "qwerty"}
// {"type": "S_IN", "name": "kali", "passw": "qwerty"}
// {"type": "S_IN", "name": "geralt", "passw": "qwerty"}
// {"type": "S_MES", "gid": -1, "did": -2, "uid": 2, "uid2": 1, "msg": "Hello I'm Geralt", "time": 3819524, "file": ""}
// {"type": "S_OUT", "id": 1}
// {"type": "S_OUT", "id": 2}

static void communicate(SSL *ssl, void *data) {
    int bytes_read = 0;
    char buff[MX_MAX];
    int n = 0;

    while(1) {
        mx_show_server_certs(ssl);
        mx_printstr("Enter the string: ");
        while ((buff[n++] = getchar()) != '\n')
            ;
        n = 0;
        SSL_write(ssl, buff, strlen(buff));
        bzero(buff, sizeof(buff));
        bytes_read = SSL_read(ssl, buff, sizeof(buff));
        buff[bytes_read] = '\0';
        if (bytes_read <= 0) {
            mx_printstr_endl("Server not responding...");
            close(SSL_get_fd(ssl));
            mx_reconnect(ssl, data);
        }
        mx_printstr_endl(buff);
    }
}

static int create_socket(void) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0) {
        mx_print_error_endl("Socket creation failed!");
        exit(MX_FAILURE);
    }
    else
        mx_printstr_endl("Socket successfully created!");

    return socket_fd;
}

static int open_connection(char *ip, int port) {
    int socket_fd = create_socket();
    struct sockaddr_in server_address;

    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip);
    server_address.sin_port = htons(port);

    if (connect(socket_fd, (MX_SA*)&server_address, sizeof(server_address))) {
        close(socket_fd);
        mx_print_error_endl("Connection with the server failed!");
        exit(MX_FAILURE);
    }
    else
        mx_printstr_endl("Connected to the server!");

    return socket_fd;
}

void mx_reconnect(SSL *ssl, void *data) {
    t_thread_data *thread_data = (t_thread_data *)data;
    int socket_fd = open_connection(thread_data->ip, thread_data->port);;

    SSL_set_fd(ssl, socket_fd);
    if (SSL_connect(ssl) == MX_SSL_FAILURE)
        ERR_print_errors_fp(stderr);
    else {
        communicate(ssl, data);
        SSL_free(ssl);
    }
    close(socket_fd);
}

void *mx_start_client(void *data) {
    t_thread_data *thread_data = (t_thread_data *)data;
    int socket_fd = -1;
    SSL_CTX *ctx = NULL;
    SSL *ssl = NULL;

    SSL_library_init();
    ctx = mx_init_client_ctx();
    socket_fd = open_connection(thread_data->ip, thread_data->port);
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, socket_fd);

    if (SSL_connect(ssl) == MX_SSL_FAILURE)
        ERR_print_errors_fp(stderr);
    else {
        communicate(ssl, data);
        SSL_free(ssl);
    }
    close(socket_fd);
    SSL_CTX_free(ctx);
    return data;
}
