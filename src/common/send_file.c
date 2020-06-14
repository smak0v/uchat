#include "uchat.h"

// from cient
void mx_send_file(int socket_fd, char *path) {
    FILE *file;
    char buffer[MX_MAX];
    int b;

    if (!(file = fopen(path, "r"))) {
        printf("%s", path);
        mx_terminate("open");
    }

    while ((b = fread(buffer, 1, sizeof(buffer), file)) > 0)
        send(socket_fd, buffer, b, 0);

    if (fclose(file) < 0)
        mx_terminate("close");
}

// from server
void mx_recv_file(int connection_fd, char *path) {
    FILE *file;
    char buffer[MX_MAX];
    int b = 0;
    int total = 0;

    read(connection_fd, buffer, MX_MAX);
    if (!(file = fopen(path, "w")))
        mx_terminate("open");

    while ((b = recv(connection_fd, buffer, 1024, 0)) > 0) {
        total += b;
        fwrite(buffer, 1, b, file);
    }

    if (fclose(file) < 0)
        mx_terminate("close");
}
