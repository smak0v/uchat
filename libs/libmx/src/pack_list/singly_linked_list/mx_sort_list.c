#include "libmx.h"

t_list *mx_sort_list(t_list *list, bool (*cmp)(void *a, void *b)) {
    t_list *help = NULL;
    t_list *store = list;
    t_list *tmp = list;
    void *swap_data;

    while (list) {
        help = store;
        while (help) {
            if ((help->next) && (cmp(help->data, help->next->data))) {
                swap_data = help->data;
                help->data = help->next->data;
                help->next->data = swap_data;
            }
            help = help->next;
        }
        list = list->next;
    }
    return tmp;
}
