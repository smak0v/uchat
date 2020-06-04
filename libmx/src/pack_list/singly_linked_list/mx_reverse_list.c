#include "libmx.h"

void mx_revers_list(t_list **list) {
    t_list *curr = *list;
    t_list *prev = NULL;
    t_list *next = NULL;

    while (curr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    *list = prev;
}
