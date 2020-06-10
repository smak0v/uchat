#include "libmx.h"

void mx_push_second(t_list **list, void *data) {
    t_list *new = mx_create_node(data);
    t_list *node = *list;

    new->next = node->next;
    node->next = new;
}
