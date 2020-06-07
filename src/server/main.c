#include "uchat.h"

void func(int socket_fd) {
    // TODO Refactor

    char buff[MX_MAX];
    int n;

    while(1) {
        bzero(buff, MX_MAX);
        read(socket_fd, buff, sizeof(buff));
        mx_printstr_endl(buff);

        bzero(buff, MX_MAX);
        n = 0;

        while ((buff[n++] = getchar()) != '\n')
            ;

        write(socket_fd, buff, sizeof(buff));

        if (strncmp("exit", buff, 4) == 0) {
            mx_printstr_endl("Server Exit!");
            break;
        }
    }
}

int mx_start_server(int port) {
    // TODO Refactor
    int socket_fd;
    int connection_fd;
    unsigned int len;
    struct sockaddr_in server_address;
    struct sockaddr_in cli;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        mx_print_error_endl("Socket creation failed!");
        exit(1);
    }
    else
        mx_printstr_endl("Socket successfully created!");

    bzero(&server_address, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);

    if ((bind(socket_fd, (MX_SA*)&server_address, sizeof(server_address))) != 0) {
        mx_print_error_endl("Socket bind failed!");
        exit(1);
    }
    else
        mx_printstr_endl("Socket successfully binded!");

    if ((listen(socket_fd, 5)) != 0) {
        mx_print_error_endl("Listen failed!");
        exit(1);
    }
    else
        mx_printstr_endl("Server listening!");

    len = sizeof(cli);

    connection_fd = accept(socket_fd, (MX_SA*)&cli, &len);
    if (connection_fd < 0) {
        mx_print_error_endl("Server acccept failed!");
        exit(1);
    }
    else
        mx_printstr_endl("server acccept the client!");

    func(connection_fd);

    close(socket_fd);

    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        mx_print_error_endl("uchat_server: must take parameter - port to run");
        mx_print_error_endl("usage: ./uchat_server PORT");
        exit(1);
    }
    else {
        if (!mx_check_port(argv[1])) {
            mx_print_error_endl("uchat_server: not valid port");
            exit(1);
        }

        long int e[50];
        long int d[50];
        long int m[50];
        long int en[50];
        mx_encryption_key(e, d);
        for (int i = 0; i < 50; ++i)
            mx_printint_endl(e[i]);
        mx_printstr_endl("--------------------");
        for (int i = 0; i < 50; ++i)
            mx_printint_endl(d[i]);
        mx_encrypt("Hello", e, m, en);
        mx_decrypt(d, m, en);
        // mx_start_server(mx_atoi(argv[1]));
    }

    return 0;
}
