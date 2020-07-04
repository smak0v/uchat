#include "server.h"

sqlite3 *mx_opendb(char *name) {
    sqlite3 *db = NULL;
    int rv = 0;

    if ((rv = sqlite3_open(name, &db)) != SQLITE_OK)
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    else
        fprintf(stderr, "Opened database successfully\n");

    mx_new_table_group_members(db);
    mx_new_table_user(db);
    mx_new_table_group(db);
    mx_new_table_dialog(db);
    mx_new_table_message(db);
    mx_new_table_socket(db);

    return db;
}

void mx_closedb(sqlite3 *db) {
    int val = sqlite3_close(db);

    if (val != SQLITE_OK)
        fprintf(stderr, "Can't close database: %s\n", sqlite3_errmsg(db));
    else
        fprintf(stderr, "Closed database successfully\n");
}

static int callback(void *data, int argc, char **argv, char **az_col_name){
    fprintf(stderr, "%s: ", (const char *)data);

    for(int i = 0; i < argc; ++i)
        printf("%s = %s\n", az_col_name[i], argv[i] ? argv[i] : "NULL");

    printf("\n");

    return 0;
}

void mx_print_db(sqlite3 *db, char *table) {
    char *sql = mx_strjoin("SELECT * from ", table);
    char *z_err_msg = 0;
    const char *data = table;
    int rc = 0;

    mx_printstr("===============================\n\n");

    rc = sqlite3_exec(db, sql, callback, (void *)data, &z_err_msg);

    mx_printstr("===============================\n");
    sqlite3_free(sql);
}

void mx_print_list_members(t_list *mem) {
    if (mem == NULL)
        mx_printstr_endl("list null");
    for (t_list *n = mem; n != NULL; n = n->next) {
        t_gr_members *g = n->data;
        mx_printint(g->gr_members_id);
        mx_printstr(" ");
        mx_printint(g->user_id);
        mx_printstr(" ");
        mx_printint(g->group_id);
        mx_printstr(" ");
        printf("%s\n", g->adm ? "true" : "false" );
        mx_printstr("\n");
    }
}
