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

static sqlite3_stmt *bind_stmt(sqlite3 *db, int sock) {
    sqlite3_stmt *stmt = NULL;
    char *num = mx_itoa(sock);
    char *sql = mx_strjoin("SELECT * FROM SOCKETS WHERE SOCKET_FD LIKE '%",
                           num);
    char *req = mx_strjoin(sql, "%'");
    int rv = sqlite3_prepare_v2(db, req, -1, &stmt, NULL);

    if (rv != SQLITE_OK)
        stmt = NULL;
    free(num);
    free(req);
    free(sql);
    return stmt;
}

static int find_sock(sqlite3 *db, sqlite3_stmt *stmt, int uid, int sock) {
    char *str = strdup((char*)sqlite3_column_text(stmt, 1));
    int len = mx_strlen(str);
    int *nums = mx_parse_sock_str(db, uid, &len);

        for (int i = 0; nums[i]; i++) {
            if (nums[i] == sock) {
                free(nums);
                free(str);
                return uid;
            }
        }
    free(nums);
    free(str);
    return -1;
}

int mx_get_user_id_by_socket(sqlite3 *db, int sock) {
    sqlite3_stmt *stmt = bind_stmt(db, sock);
    int uid = -1;

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        uid = sqlite3_column_int(stmt, 0);
        if ((uid = find_sock(db, stmt, uid, sock)) > 0)
            break;
    }
    sqlite3_finalize(stmt);
    return uid;
}
