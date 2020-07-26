#include "client.h"

t_main_thread *mx_create_main_thread_struct(char *response, t_glade *g) {
    t_main_thread *main_thread = mx_memalloc(sizeof(t_main_thread));

    main_thread->response = mx_strdup(response);
    main_thread->g = g;

    return main_thread;
}

void mx_delete_main_thread_struct(t_main_thread **main_thread) {
    char *response = (*main_thread)->response;

    mx_strdel(&response);

    free(*main_thread);
    *main_thread = NULL;
}
