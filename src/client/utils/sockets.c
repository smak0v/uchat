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
    thread_data++;
    // TODO Forever llo that listen to the server and display all input responses

    // int bytes_read = 0;
    // char buff[MX_MAX];
    // // int n = 0;

    // while (1) {
    //     // mx_show_server_certs(thread_data->ssl);
    //     // mx_printstr("Enter the string: ");
    //     // while ((buff[n++] = getchar()) != '\n')
    //     //     ;
    //     // n = 0;
    //     // SSL_write(thread_data->ssl, buff, strlen(buff));
    //     bzero(buff, sizeof(buff));
    //     bytes_read = SSL_read(thread_data->ssl, buff, sizeof(buff));
    //     buff[bytes_read] = '\0';
    //     if (bytes_read <= 0) {
    //         mx_printstr_endl("Server not responding...");
    //         close(SSL_get_fd(thread_data->ssl));
    //     }
    //     mx_printstr_endl(buff);
    // }
    return data;
}

void mx_start_client(char *ip, int port, t_glade *g) {
    int socket_fd = -1;
    SSL_CTX *ctx = NULL;
    SSL *ssl = NULL;

    SSL_library_init();
    ctx = mx_init_client_ctx();
    g->ctx = ctx;
    if ((socket_fd = mx_open_connection(ip, port)) < 0)
        mx_terminate("Connection error"); // change this to try to reconnect
    ssl = SSL_new(ctx);
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
    SSL_CTX_free(ctx);
}
