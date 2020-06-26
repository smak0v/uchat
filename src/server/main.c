#include "server.h"

void *mx_communicate(void *data) {
    t_comm *connect = (t_comm *)data;
    char buff[MX_MAX];
    char *status = connect->status;
    int bytes_read = 0;
    char *response = NULL;

    free(connect);
    while (1) {
        bzero(buff, MX_MAX);
        bytes_read = read(connect->fd, buff, sizeof(buff));
        if (bytes_read <= 0 || mx_strcmp(buff, "exit\n") == 0) {
            close(connect->fd);
            *status = 0;
            printf("Connection closed\n");
            pthread_exit(NULL);
        }
        response = mx_process_request(buff, connect->db, connect->fd);
        // printf("%s\n", response);
        write(connect->fd, response, sizeof(char) * mx_strlen(response));
    }
}

void accept_clients(int socket_fd, sqlite3 *db) {
    int connect_fd = 0;
    unsigned int len = 0;
    struct sockaddr_in client_addr;
    t_meta *trd_data = mx_init_threads(db);

    while (1) {
        printf("Ready for new client\n");
        len = sizeof(client_addr);
        connect_fd = accept(socket_fd, (MX_SA *)&client_addr, &len);
        if (connect_fd < 0)
            mx_terminate("Server acccept failed!");
        mx_thread_manager(connect_fd, &trd_data);
    }
}

int mx_start_server(int port) {
    int socket_fd = 0;
    struct sockaddr_in server_addr;
    sqlite3 *db = NULL;

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

    db = mx_opendb("test.db");
    accept_clients(socket_fd, db);
    mx_closedb(db);

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

    pthread_exit(NULL);
}
