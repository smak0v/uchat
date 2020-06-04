#include "libmx.h"

t_list *mx_list_insertion_sort(t_list *list, bool (*cmp)(void *a, void *b)) {
    t_list *sorted = NULL;
    t_list *head = NULL;
    t_list **trail = NULL;

    if (!list || !list->next)
        return list;
    while (list) {
        head  = list;
        trail = &sorted;
        list = list->next;
        while (!(*trail == NULL || !cmp(head->data, (*trail)->data)))
            trail = &(*trail)->next;
        head->next = *trail;
        *trail = head;
    }
    return sorted;
}
