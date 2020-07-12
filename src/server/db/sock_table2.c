#include "server.h"

int mx_update_socket_by_user_id(sqlite3 *db, char *socket, int user_id) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db,
            "UPDATE SOCKETS SET SOCKET_FD = ?1 WHERE USER_ID = ?2;",
            -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, socket, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, user_id);

    if (rv == SQLITE_ERROR)
        return -1;

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;

    sqlite3_finalize(stmt);

    return 0;
}
