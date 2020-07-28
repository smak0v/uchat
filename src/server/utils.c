#include "server.h"

static char *process_path(char *file, int msg_id, bool type) {
    char *itoa_str = NULL;
    char *tmp = NULL;
    char *path = NULL;

    itoa_str = mx_itoa(msg_id);
    tmp = mx_strjoin(itoa_str, "_");
    mx_strdel(&itoa_str);
    if (type == 0) {
        itoa_str = mx_memrchr(file, '/', (size_t)mx_strlen(file));
        itoa_str += 1;
        path = mx_strjoin(tmp, itoa_str);
    }
    else
        path = mx_strjoin(tmp, file);

    mx_strdel(&tmp);
    tmp = mx_strjoin("files/", path);
    mx_strdel(&path);

    return tmp;
}

FILE *mx_open_file(char *filename, char *mode) {
    FILE *file = NULL;
    struct stat st;
    char *path = mx_strjoin(MX_DOWNLOADS, filename);

    if (stat(MX_DOWNLOADS, &st) == -1)
        mkdir(MX_DOWNLOADS, 0700);
    if (path)
        file = fopen(path, mode);
    mx_strdel(&path);

    return file;
}

t_auditor *mx_auditor_kostyl(int mid, int port, char *file, bool type) {
    t_auditor *data = malloc(sizeof(t_auditor));

    data->mid = mid;
    data->port = port;
    data->file = file;
    data->mode = type;

    return data;
}

t_ft_data *mx_init_ft_data(t_comm *connect, int socket, t_auditor *kostyl) {
    t_ft_data *data = malloc(sizeof(t_ft_data));

    data->name = process_path(kostyl->file, kostyl->mid, kostyl->mode);
    data->sock = socket;
    data->status = &((connect->status)[kostyl->port - 7200]);
    data->type = kostyl->mode;

    free(kostyl);
    return data;
}

void mx_close_connection(t_comm *connect, char *status) {
    int uid = -1;

    close(SSL_get_fd(connect->ssl));
    *status = 0;
    ERR_print_errors_fp(stderr);

    uid = mx_get_user_id_by_socket(connect->db, connect->fd);
    mx_remove_socket(connect->db, connect->fd, uid);

    mx_pop_from_ssl_list(connect->ssl_list, connect->fd);

    mx_printstr_endl("Connection closed");
    pthread_exit(NULL);
}
