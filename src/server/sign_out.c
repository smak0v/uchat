#include "server.h"

char *mx_sign_out(void *jobj, sqlite3 *db, int fd) {
    fd = 0;
    db = 0;
    jobj = 0;
    return "{\"code\": 200}";
}
