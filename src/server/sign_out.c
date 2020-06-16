#include "server.h"

char *mx_sign_out(void *jobj, t_list **clients, sqlite3 *db, int fd) {
    mx_pop_client(clients, fd);

    db = 0;
    jobj = 0;
    return "{\"code\": 200}";
}
