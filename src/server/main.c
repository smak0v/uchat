#include "uchat.h"

void func(int socket_fd) {
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


int main() {
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
    server_address.sin_port = htons(MX_PORT);

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
