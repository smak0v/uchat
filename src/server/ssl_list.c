#include "server.h"

void mx_pop_from_ssl_list(t_list **ssl_list, int delete_fd) {
    SSL *ssl = NULL;
    t_list *list = *ssl_list;
    t_list *prev_list = NULL;
    int fd = -1;

    while (list) {
        ssl = (SSL *)list->data;
        fd = SSL_get_fd(ssl);

        if (fd == delete_fd) {
            if (prev_list)
                prev_list->next = list->next;
            else
                *ssl_list = list->next;
            free(list);
        }

        prev_list = list;
        list = list->next;
    }
}

SSL *mx_find_ssl(t_list **ssl_list, int fd) {
    SSL *ssl = NULL;
    t_list *list = *ssl_list;

    while (list) {
        ssl = (SSL *)list->data;

        if (SSL_get_fd(ssl) == fd)
            return ssl;

        list = list->next;
    }

    return NULL;
}
