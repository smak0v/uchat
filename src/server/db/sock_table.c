#include "server.h"

void mx_new_table_socket(sqlite3 *database) {
    sqlite3_stmt *stmt = NULL;

    sqlite3_prepare_v2(database, "DELETE FROM SOCKETS", -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_exec(database, "CREATE TABLE IF NOT EXISTS SOCKETS(" \
                 "USER_ID INTEGER PRIMARY KEY NOT NULL," \
                 "SOCKET_FD TEXT NOT NULL," \
                 "TOKEN TEXT NOT NULL," \
                 "FOREIGN KEY (USER_ID) REFERENCES USERS (USER_ID));",
                 0, 0, 0);
}

int mx_add_sock_user(sqlite3 *db, int user_id,
                     char *sock_fd, char *token) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db,
        "INSERT INTO SOCKETS(USER_ID, SOCKET_FD, TOKEN)" \
        "VALUES(?1, ?2, ?3);", -1, &stmt, NULL);

    if (rv == SQLITE_ERROR)
        return -1;
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_text(stmt, 2, sock_fd, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, token, -1, SQLITE_STATIC);
    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return -2;
    }
    sqlite3_finalize(stmt);
    return 0;
}

char *mx_get_sock_by_user_id(sqlite3 *db, int user_id) {
    sqlite3_stmt *stmt = NULL;
    char *socket_fd = NULL;
    int rv = sqlite3_prepare_v2(db,
        "SELECT * FROM SOCKETS WHERE USER_ID = ?1", -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, user_id);

    if (rv != SQLITE_OK)
        return mx_strnew(0);
    if ((rv = sqlite3_step(stmt)) != SQLITE_ROW)
        if (rv == SQLITE_ERROR)
            return mx_strnew(0);
    if (sqlite3_column_text(stmt, 1) != NULL)
        socket_fd = strdup((char*)sqlite3_column_text(stmt, 1));

    sqlite3_finalize(stmt);

    return socket_fd;
}

char *mx_get_token_by_user_id(sqlite3 *db, int user_id) {
    sqlite3_stmt *stmt = NULL;
    char *token = NULL;
    int rv = sqlite3_prepare_v2(db,
        "SELECT * FROM SOCKETS WHERE USER_ID = ?1", -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, user_id);

    if (rv != SQLITE_OK)
        return mx_strnew(0);
    if ((rv = sqlite3_step(stmt)) != SQLITE_ROW)
        if (rv == SQLITE_ERROR)
            return mx_strnew(0);
    if (sqlite3_column_text(stmt, 2) != NULL)
        token = strdup((char *)sqlite3_column_text(stmt, 2));

    sqlite3_finalize(stmt);

    return token;
}

int mx_delete_sock_by_user_id(sqlite3 *db, int user_id) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db, "DELETE FROM SOCKETS WHERE USER_ID = ?1;",
        -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, user_id);

    if (rv == SQLITE_ERROR)
        return -1;

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;

    sqlite3_finalize(stmt);

    return 0;
}
