#include "server.h"

void mx_send_to_all_clients(sqlite3 *db, char *j_str, int uid) {
    int len = 0;
    int *sockets = mx_parse_sock_str(db, uid, &len);

    for (int i = 0; i < len; ++i)
        write(sockets[i], j_str, (sizeof(char) * strlen(j_str)));

}
