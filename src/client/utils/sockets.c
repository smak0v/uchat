#include "client.h"

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

int mx_open_connection(char *ip, int port) {
    int socket_fd = create_socket();
    struct sockaddr_in server_address;

    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip);
    server_address.sin_port = htons(port);

    if (connect(socket_fd, (MX_SA*)&server_address, sizeof(server_address))) {
        close(socket_fd);
        mx_print_error_endl("Connection with the server failed!");
        return -1;
    }
    else
        mx_printstr_endl("Connected to the server!");

    return socket_fd;
}

void *mx_listen_server(void *data) {
    t_thread_data *thread_data = (t_thread_data *)data;
    char *response = NULL;

    while (1) {
        response = mx_read_server_response(thread_data->glade);

        mx_printstr_endl(response);

        if (response)
            mx_check_response_type(response, thread_data->glade);

        mx_strdel(&response);
    }

    return data;
}

void mx_start_client(char *ip, int port, t_glade *g) {
    int socket_fd = -1;
    SSL *ssl = NULL;

    SSL_library_init();
    g->ctx = mx_init_client_ctx();
    if ((socket_fd = mx_open_connection(ip, port)) < 0)
        mx_terminate("Connection error");
    ssl = SSL_new(g->ctx);
    SSL_set_fd(ssl, socket_fd);
    if (SSL_connect(ssl) == MX_SSL_FAILURE)
        ERR_print_errors_fp(stderr);
    else {
        g->ssl = ssl;
        mx_client_thread_manager(g, ssl);
        gtk_main();
        SSL_free(ssl);
    }
    close(socket_fd);
    SSL_CTX_free(g->ctx);
}
