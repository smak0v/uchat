#include "server.h"

int mx_delete_user_by_login(sqlite3 *db, char *login) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db, "DELETE FROM USER WHERE LOGIN = ?1;",
        -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, login, -1, SQLITE_STATIC);

    if (rv == SQLITE_ERROR)
        return -1;

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;

    sqlite3_finalize(stmt);

    return 0;
}

int mx_delete_user_by_id(sqlite3 *db, int user_id) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db, "DELETE FROM USER WHERE USER_ID = ?1;",
        -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, user_id);

    if (rv == SQLITE_ERROR)
        return -1;

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;

    sqlite3_finalize(stmt);

    return 0;
}
