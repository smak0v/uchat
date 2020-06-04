#include "libmx.h"

t_tree *mx_create_tree_node(void *data) {
    t_tree *node = malloc(sizeof(t_tree));

    node->data = data;
    node->left = node->right = NULL;
    return node;
}
