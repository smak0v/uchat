#include "libmx.h"

void mx_dll_push_front(t_dll **list, void *data) {
    t_dll *node = mx_dll_create_node(data);

    node->next = *list;
    if (*list)
        (*list)->prev = node;
    *list = node;
}
