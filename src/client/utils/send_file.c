#include "client.h"

void mx_send_file(SSL *ssl, char *path) {
    FILE *file;
    char buffer[1024];
    int b = 1;
    char *json_str;
    int pack_num = 1;

    if (!(file = fopen(path, "r")))
        mx_terminate("open");

    while ((b = fread(buffer, sizeof(char), sizeof(buffer), file)) > 0) {
        json_str = mx_json_string_s_file(1, pack_num++, buffer, b);
        SSL_write(ssl, json_str, strlen(json_str));
        mx_strdel(&json_str);
        usleep(1000);
        bzero(buffer, sizeof(buffer));
    }

    if (fclose(file) < 0)
        mx_terminate("close");
}
