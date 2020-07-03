#include "uchat.h"

// from cient
void mx_send_file(int socket_fd, char *path) {
    FILE *file;
    char buffer[1024];
    int b = 1;

    if (!(file = fopen(path, "r")))
        mx_terminate("open");

    while ((b = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        mx_printstr(buffer);
        send(socket_fd, buffer, b, 0);
        bzero(buffer, sizeof(buffer));
    }

    if (fclose(file) < 0)
        mx_terminate("close");
}

// from server
void mx_recv_file(int connection_fd, char *path) {
    FILE *file;
    char buffer[1024];
    int b = 1;
    char *full_path = mx_strjoin("files/", path);

    if (!(file = fopen(full_path, "w")))
        mx_terminate("open");

    while ((b = recv(connection_fd, buffer, sizeof(buffer), 0)) > 0) {
        mx_printstr(buffer);
        fwrite(buffer, 1, b, file);
    }

    if (fclose(file) < 0)
        mx_terminate("close");

    mx_strdel(&full_path)
}
