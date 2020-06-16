#include "uchat.h"

int mx_delete_user_from_group(sqlite3 *db, int user_id, int group_id) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db,
        "DELETE FROM GROUP_MEMBERS WHERE USER_ID = ?1 AND GROUP_ID = ?2;\
        DELETE FROM USER WHERE USER_ID = ?1", -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, group_id);

    if (rv == SQLITE_ERROR)
        return -1;

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;

    sqlite3_finalize(stmt);

    return 1;
}
