#include "server.h"

static void *mx_receive_file(void *void_data) {
    t_ft_data *data = (t_ft_data *)void_data;
    struct sockaddr_in cli_addr;
    unsigned int len = sizeof(cli_addr);
    int connect = accept(data->sock, (MX_SA *)&cli_addr, &len);
    SSL *ssl = NULL;
    printf("1\n");
    if (connect < 0)
        pthread_exit(NULL);
printf("2\n");
    ssl = SSL_new(data->ctx);
    SSL_set_fd(ssl, connect);
printf("3\n");
    mx_recv_file(ssl, data->name);
    printf("4\n");
    SSL_free(ssl);
    pthread_exit(NULL);
}

char *mx_file_transfer(SSL_CTX *ctx, char *file, char *res) {
    int socket = mx_open_listener(MX_FT_PORT);
    pthread_t *thr = NULL;
    t_ft_data *data = NULL;

    printf("Initiating file transfer\n");
    if (socket > 0) {
        thr = malloc(sizeof(pthread_t));
        data = malloc(sizeof(t_ft_data));
        data->name = file;
        data->ctx = ctx;
        data->sock = socket;
        printf("creating a thread\n");
        if (pthread_create(thr, NULL, mx_receive_file, (void *)data) != 0)
            return "{\"code\": 666}";
        printf("finished");
        return res;
    }
    else
        return "{\"code\": 666}";
}

