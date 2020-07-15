#include "client.h"

static void *send_file_thread(void *void_data) {
    t_ft_data *data = (t_ft_data *)void_data;
    SSL *ssl = SSL_new(data->ctx);
    SSL_set_fd(ssl, data->sock);
    printf("1\n");
    if (SSL_connect(ssl) == MX_SSL_FAILURE) {
        ERR_print_errors_fp(stderr);
        pthread_exit(NULL);
    }
    else
        mx_send_file(ssl, data->name);

    pthread_exit(NULL);
}

void mx_process_send_file(t_glade *g, char *path) {
    int connection_fd = -1;
    pthread_t *thr = NULL;
    t_ft_data *data = NULL;

    printf("Process send file started\n");
    connection_fd = mx_open_connection(g->ip, MX_FT_PORT);
    if (connection_fd < 0)
        return;
    printf("Got a connection fd!\n");
    thr = malloc(sizeof(pthread_t));
    data = malloc(sizeof(t_ft_data));
    data->name = path;
    data->ctx = g->ctx;
    data->sock = connection_fd;

    printf("Creating a thread\n");
    if (pthread_create(thr, NULL, send_file_thread, (void *)data) != 0)
        printf("Thread creation error in process_send_file\n");
    printf("mx_send_file finished\n");
}

void mx_send_file(SSL *ssl, char *path) {
    FILE *file;
    char buffer[1024];
    int b = 1;
    char *json_str;
    int pack_num = 1;
    printf("2\n");
    if (!(file = fopen(path, "r")))
        mx_terminate("open");
    printf("3\n");
    while ((b = fread(buffer, sizeof(char), sizeof(buffer), file)) > 0) {
        json_str = mx_json_string_s_file(1, pack_num++, buffer, b);
        SSL_write(ssl, json_str, strlen(json_str));
        mx_strdel(&json_str);
        usleep(1000);
        bzero(buffer, sizeof(buffer));
    }
    printf("4\n");
    if (fclose(file) < 0)
        mx_terminate("close");
}
