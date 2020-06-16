#include "uchat.h"

int mx_change_user_pass(sqlite3 *db, char *login, char *new_pass) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db, "UPDATE USER SET PASSWORD = ?1 WHERE " \
                                "LOGIN = ?2;", -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, new_pass, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, login, -1, SQLITE_STATIC);

    if (rv == SQLITE_ERROR)
        return -1;

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;

    sqlite3_finalize(stmt);

    return 1;
}
