#include "libmx.h"

void mx_dll_push_back(t_dll **list, void *data) {
    t_dll *node = mx_dll_create_node(data);
    t_dll *last = *list;

    if (!last) {
        *list = node;
        return;
    }
    while (last->next)
        last = last->next;
    last->next = node;
    node->prev = last;
}
