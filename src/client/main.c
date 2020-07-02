#include "uchat.h"

static void communicate(SSL *ssl, int socket_fd) {
    int bytes_read = 0;
    char buff[MX_MAX];
    int n = 0;

    while(1) {
        mx_show_server_certs(ssl);
        mx_printstr("Enter the string: ");
        while ((buff[n++] = getchar()) != '\n');
        n = 0;

        if (buff[0] == '1') {
            while (1) {
                bytes_read = read(socket_fd, buff, sizeof(buff));
                buff[bytes_read] = '\0';
                mx_printstr_endl(buff);
                char f = getchar();
                if (f == '2')
                    break;
            }
        }

        SSL_write(ssl, buff, strlen(buff));
        bzero(buff, sizeof(buff));
        bytes_read = SSL_read(ssl, buff, sizeof(buff));
        buff[bytes_read] = '\0';
        mx_printstr_endl(buff);
        // if ((strncmp(buff, "exit", 4)) == 0) {
        //     mx_printstr_endl("Client Exit...");
        //     break;
        // }
    }
}

static int open_connection(char *ip, int port) {
    struct sockaddr_in server_address;
    struct hostent *host = NULL;
    int socket_fd = -1;

    if ((host = gethostbyname(ip)) == NULL) {
        perror(ip);
        exit(MX_FAILURE);
    }
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        mx_print_error_endl("Socket creation failed!");
        exit(MX_FAILURE);
    }
    else
        mx_printstr_endl("Socket successfully created!");
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
        communicate(ssl, socket_fd);
        SSL_free(ssl);
    }
    close(socket_fd);
    SSL_CTX_free(ctx);
    return data;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        mx_print_error("uchat: must take two parameters -");
        mx_print_error_endl("server IP address and port");
        mx_print_error_endl("usage: ./uchat IP PORT");
        exit(MX_FAILURE);
    }
    else {
        if (!mx_check_ip(strdup(argv[1]))) {
            mx_print_error_endl("uchat: not valid IP address");
            exit(MX_FAILURE);
        }
        if (!mx_check_port(argv[2])) {
            mx_print_error_endl("uchat: not valid port");
            exit(MX_FAILURE);
        }
        mx_init_client(argc, argv);
    }

    return MX_SUCCESS;
}


// {"type": "REG", "name": "kali", "passw": "qwerty"}
// {"type": "REG", "name": "geralt", "passw": "qwerty"}
// {"type": "S_IN", "name": "kali", "passw": "qwerty"}
// {"type": "S_IN", "name": "geralt", "passw": "qwerty"}
// {"type": "S_MES", "gid": -1, "did": -2, "uid": 2, "uid2": 1, "msg": "Hello I'm Geralt", "time": 3819524, "file": ""}
// {"type": "S_OUT", "id": 1}
// {"type": "S_OUT", "id": 2}
