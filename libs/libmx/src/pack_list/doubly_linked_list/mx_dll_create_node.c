#include "libmx.h"

t_dll *mx_dll_create_node(void *data) {
    t_dll *node = (t_dll *)malloc(sizeof(t_dll));

    node->data = data;
    node->next = node->prev = NULL;
    return node;
}
