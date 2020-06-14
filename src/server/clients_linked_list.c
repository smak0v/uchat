#include "server.h"

void mx_add_client(t_list **clients, int connection_fd, char *uname, int uid) {
    t_cli *client = malloc(sizeof(t_cli));
    printf("%d, %d, %s\n", connection_fd, uid, uname);
    client->connection_fd = connection_fd;
    client->user_id = uid;
    client->username = uname;
    printf("2\n");
    mx_push_front(clients, (void *)client);
    printf("3\n");
}

void mx_pop_client(t_list **clients, int connection_fd) {
    if (!clients)
        return;

    t_list *list = *clients;
    t_list *prev = NULL;

    while (list) {
        if (((t_cli *)(list->data))->connection_fd == connection_fd) {
            if (prev)
                prev->next = list->next;
            else
                *clients = list->next;
            free(list);
            break;
        }
        prev = list;
        list = list->next;
    }
}

t_cli *mx_find_client_by_uname(t_list **clients, char *uname) {
    if (!clients)
        return NULL;

    t_list *list = *clients;
    char *string = NULL;

    while (list) {
        string = ((t_cli *)(list->data))->username;
        if (!mx_strcmp(string, uname))
            return list->data;
        list = list->next;
    }

    return NULL;
}

t_cli *mx_find_client_by_uid(t_list **clients, int uid) {
    if (!clients)
        return NULL;
    
    t_list *list = *clients;

    while (list) {
        if (((t_cli *)(list->data))->user_id == uid)
            return list->data;
        list = list->next;
    }

    return NULL;
}
