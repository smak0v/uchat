#include "libmx.h"

void mx_push_front(t_list **list, void *data) {
    t_list *new = mx_create_node(data);

    if (*list) {
        new->next = *list;
        *list = new;
    }
    else
        *list = new;
}
