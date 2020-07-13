#include "server.h"

void *mx_communicate(void *data) {
    t_comm *connect = (t_comm *)data;
    char buff[MX_MAX];
    char *status = connect->status;
    int bytes_read = 0;
    char *response = NULL;

    while (1) {
        if (SSL_accept(connect->ssl) == MX_SSL_FAILURE)
            ERR_print_errors_fp(stderr);
        else {
            bzero(buff, MX_MAX);
            bytes_read = SSL_read(connect->ssl, buff, sizeof(buff));
            buff[bytes_read] = '\0';
            if (bytes_read <= 0 || mx_strcmp(buff, "exit\n") == 0) {
                close(SSL_get_fd(connect->ssl));
                *status = 0;
                ERR_print_errors_fp(stderr);
                mx_printstr_endl("Connection closed");
                pthread_exit(NULL);
            }
            response = mx_process_request(buff, connect);
            SSL_write(connect->ssl, response, strlen(response));
        }
    }
}

void accept_clients(int socket_fd, sqlite3 *db, SSL_CTX *ctx) {
    SSL *ssl = NULL;
    int connect_fd = 0;
    unsigned int len = 0;
    struct sockaddr_in client_addr;
    t_meta *trd_data = mx_init_threads(db);

    while (1) {
        mx_printstr_endl("Ready for new client");
        len = sizeof(client_addr);

        connect_fd = accept(socket_fd, (MX_SA *)&client_addr, &len);
        if (connect_fd < 0)
            mx_terminate("Server acccept failed!");

        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, connect_fd);

        trd_data->ssl = ssl;
        mx_thread_manager(connect_fd, &trd_data);
    }
}

static int open_listener(int port) {
    struct sockaddr_in server_addr;
    int socket_fd = -1;

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        mx_terminate("Socket creation failed!");
    mx_printstr_endl("Socket successfully created!");

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    if ((bind(socket_fd, (MX_SA *)&server_addr, sizeof(server_addr))))
        mx_terminate("Socket bind failed!");
    mx_printstr_endl("Socket successfully binded!");
    if ((listen(socket_fd, 5)))
        mx_terminate("Listen failed!");
    mx_printstr_endl("Server listening!");

    return socket_fd;
}

int mx_start_server(int port) {
    int socket_fd = -1;
    sqlite3 *db = NULL;
    SSL_CTX *ctx = NULL;

    SSL_library_init();

    ctx = mx_init_server_ctx();
    mx_load_certificates(ctx, "./certs/cert.pem", "./certs/cert.pem");

    socket_fd = open_listener(port);
    db = mx_opendb("test.db");
    mx_print_db(db, "SOCKETS");
    accept_clients(socket_fd, db, ctx);

    mx_closedb(db);
    close(socket_fd);
    SSL_CTX_free(ctx);

    return 0;
}


static int count_commas(char *sock) {
    int counter = 0;

    for (int i = 0; sock[i]; i++) {
        if (sock[i] == ',')
            counter++;
    }
    counter += 1;
    return counter;
}

static int *parse_sock_str(char *sock, int *sock_array, int len) {
    int atoi = 0;
    int arr_counter = 0;
    int j = 0;

    for (int i = 0; arr_counter < len;) {
        while (sock[j] && sock[j] != ',')
            j++;
        if (j != i) {
            char str[j - i + 1];

            mx_strncpy(str, &sock[i], j - i);
            str[j - i] = '\0';
            atoi = mx_atoi(str);
            sock_array[arr_counter] = atoi;
            arr_counter++;
            i += (j - i + 1);
            j += 1;
        }
    }
    return sock_array;
}

int *test_parse_sock_str(char *sock, int *len) {
    // char *sock = mx_get_sock_by_user_id(db, uid);
    int *sock_array = NULL;

    if (!sock || sock[0] == '\0')
        return NULL;

    *len = count_commas(sock);
    sock_array = malloc(sizeof(int) * (*len));

    if (*len == 1) {
        sock_array[0] = mx_atoi(sock);
        return sock_array;
    }
    else
        return parse_sock_str(sock, sock_array, *len);

    return sock_array;
}

// int main(int argc, char **argv) {
//     if (argc < 2) {
//         mx_print_error_endl("uchat_server: must take parameter - port to run");
//         mx_print_error_endl("usage: ./uchat_server PORT");
//         exit(MX_FAILURE);
//     } else {
//         if (!mx_check_port(argv[1]))
//             mx_terminate("uchat_server: not valid port");
//         mx_start_server(mx_atoi(argv[1]));
//     }

//     pthread_exit(NULL);
// }

int main(int argc, char **argv) {
    int len = 0;

    argc++;
    int *arr = test_parse_sock_str(argv[1], &len);

    for (int i = 0; i < len; i++)
        mx_printint_endl(arr[i]);
    system("leaks uchat_server");
    return 0;
}
