#include "libmx.h"

bool mx_is_equal_lists(t_list *a, t_list *b) {
    t_list *a_c = a;
    t_list *b_c = b;

    while ((a_c) && (b_c)) {
        if (mx_strcmp(a_c->data, b_c->data) != 0)
            return false;
        a_c = a_c->next;
        b_c = b_c->next;
    }
    return (!a_c) && (!b_c);
}
