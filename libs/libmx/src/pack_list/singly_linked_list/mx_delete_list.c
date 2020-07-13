#include "libmx.h"

void mx_delete_list(t_list *head) {
    t_list *temp = head;

    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
