#include "server.h"

int mx_change_user_pass(sqlite3 *db, int user_id, char *new_pass) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db, "UPDATE USER SET PASSWORD = ?1 WHERE " \
                                "USER_ID = ?2;", -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, new_pass, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, user_id);

    if (rv == SQLITE_ERROR)
        return -1;

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;

    sqlite3_finalize(stmt);

    return 0;
}

int mx_change_user_username(sqlite3 *db, int user_id, char *username) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db,
            "UPDATE USER SET LOGIN = ?1 WHERE USER_ID = ?2;",
            -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, user_id);

    if (rv == SQLITE_ERROR)
        return -1;

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;

    sqlite3_finalize(stmt);

    return 0;
}
