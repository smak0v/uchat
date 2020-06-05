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

int main() {
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
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(MX_PORT);

    if (connect(socket_fd, (MX_SA*)&server_address, sizeof(server_address)) != 0) {
        mx_print_error_endl("Connection with the server failed!");
        exit(0);
    }
    else
        mx_printstr_endl("Connected to the server!");

    func(socket_fd);

    close(socket_fd);
}
