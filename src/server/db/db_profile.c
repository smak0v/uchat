#include "server.h"

void mx_new_table_profiles(sqlite3 *database) {
    sqlite3_exec(database, "CREATE TABLE IF NOT EXISTS PROFILES(" \
                 "USER_ID INTEGER PRIMARY KEY NOT NULL," \
                 "NAME TEXT NOT NULL," \
                 "BIRTH TEXT," \
                 "EMAIL TEXT," \
                 "STATUS TEXT," \
                 "COUNTRY TEXT);", 0, 0, 0);
}

int mx_add_profile(sqlite3 *db, t_profile *usr) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db,
            "INSERT INTO PROFILES(USER_ID, NAME, BIRTH, EMAIL, STATUS, \
            COUNTRY)VALUES(?1, ?2, ?3, ?4, ?5, ?6);", -1, &stmt, NULL);

    if (rv == SQLITE_ERROR)
        return -1;
    sqlite3_bind_int(stmt, 1, usr->user_id);
    sqlite3_bind_text(stmt, 2, usr->name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, usr->birth, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, usr->email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, usr->status, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, usr->country, -1, SQLITE_STATIC);
    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return -1;
    }
    sqlite3_finalize(stmt);
    return 0;
}


int mx_delete_profile(sqlite3 *db, int user_id) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db,
            "DELETE FROM PROFILES WHERE USER_ID = ?1;", -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, user_id);

    if (rv == SQLITE_ERROR)
        return -1;

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;

    sqlite3_finalize(stmt);

    return 0;
}

