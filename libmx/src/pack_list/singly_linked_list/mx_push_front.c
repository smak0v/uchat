#include "libmx.h"

void mx_push_front(t_list **list, void *data) {
    t_list *new = mx_create_node(data);

    new->next = *list;
    *list = new;
}
