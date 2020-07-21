#include "server.h"

static void *mx_receive_file(void *void_data) {
    t_ft_data *data = (t_ft_data *)void_data;
    struct sockaddr_in cli_addr;
    unsigned int len = sizeof(cli_addr);
    int connect = accept(data->sock, (MX_SA *)&cli_addr, &len);

    if (connect < 0)
        pthread_exit(NULL);

    mx_recv_file(data->name, connect);

    // *(data->status) = 0;
    pthread_exit(NULL);
}

// TODO: REFACTOR
char *mx_file_transfer(t_comm *connect, char *file, char *res, int msg_id) {
    // int index = -1;
    // int port = -1;
    int socket = -1;
    pthread_t *thr = NULL;
    t_ft_data *data = NULL;

    (void)connect; // tmp line to silence the warning
    msg_id = 0; // TODO: ADD MSG_ID TO JSON
    // while (mx_get_free_thread(connect->status, &index) != 0)
    //     printf("waiting for a free thread\n");
    // port = index + 1000;
    socket = mx_open_listener(MX_FT_PORT);
    if (socket > 0) {
        thr = malloc(sizeof(pthread_t));
        data = malloc(sizeof(t_ft_data));
        data->name = mx_strdup(file);
        data->sock = socket;
        // data->status = &((connect->status)[index]);
        if (pthread_create(thr, NULL, mx_receive_file, (void *)data) != 0)
            return "{\"code\": 666}";
        return res;
    }
    else
        return "{\"code\": 666}";
}

