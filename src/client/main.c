#include "uchat.h"

void func(int socket_fd) {
    char buff[MX_MAX];
    int n;

    while(1) {
        bzero(buff, sizeof(buff));
        mx_printstr("Enter the string: ");
        n = 0;

        while ((buff[n++] = getchar()) != '\n')
            ;

        write(socket_fd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(socket_fd, buff, sizeof(buff));
        mx_printstr(buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            mx_printstr_endl("Client Exit...");
            break;
        }
    }
}

int mx_start_client(char *ip, int port) {
    int socket_fd;
    struct sockaddr_in server_address;


    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        mx_print_error_endl("Socket creation failed!");
        exit(1);
    }
    else
        mx_printstr_endl("Socket successfully created!");

    bzero(&server_address, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip);
    server_address.sin_port = htons(port);

    if (connect(socket_fd, (MX_SA*)&server_address, sizeof(server_address)) != 0) {
        mx_print_error_endl("Connection with the server failed!");
        exit(0);
    }
    else
        mx_printstr_endl("Connected to the server!");

    func(socket_fd);

    close(socket_fd);

    return 0;
}

int main(int argc, char **argv) {
    int status = MX_SUCCESS;

    if (argc < 3) {
        mx_print_error("uchat: must take two parameters -");
        mx_print_error_endl("server IP address and port");
        mx_print_error_endl("usage: ./uchat IP PORT");
        exit(1);
    }
    else {
        if (!mx_check_ip(strdup(argv[1]))) {
            mx_print_error_endl("uchat: not valid IP address");
            exit(1);
        }
        if (!mx_check_port(argv[2])) {
            mx_print_error_endl("uchat: not valid port");
            exit(1);
        }
        status = mx_init_client(argc, argv);
        // mx_start_client(argv[1], mx_atoi(argv[2]));
    }
    return status;
}
