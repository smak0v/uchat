#include "uchat.h"

void mx_handle_connection(int socket_fd) {
    char buff[MX_MAX];
    int n;

    while (1) {
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
    int socket_fd;
    int connection_fd;
    unsigned int client_addr_len;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        mx_terminate("Socket creation failed!");
    mx_printstr_endl("Socket successfully created!");
    
    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    if ((bind(socket_fd, (MX_SA *)&server_addr, sizeof(server_addr))) != 0)
        mx_terminate("Socket bind failed!");
    mx_printstr_endl("Socket successfully binded!");

    if ((listen(socket_fd, 5)) != 0)
        mx_terminate("Listen failed!");
    mx_printstr_endl("Server listening!");

    client_addr_len = sizeof(client_addr);
    connection_fd = accept(socket_fd, (MX_SA *)&client_addr, &client_addr_len);
    if (connection_fd < 0)
        mx_terminate("Server acccept failed!");
    mx_printstr_endl("Server acccept the client!");

    mx_handle_connection(connection_fd);

    close(socket_fd);

    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        mx_print_error_endl("uchat_server: must take parameter - port to run");
        mx_print_error_endl("usage: ./uchat_server PORT");
        exit(1);
    } else {
        if (!mx_check_port(argv[1]))
            mx_terminate("uchat_server: not valid port");
        mx_start_server(mx_atoi(argv[1]));
    }

    return 0;
}
