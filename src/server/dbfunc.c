#include "uchat.h"

sqlite3 *mx_opendb(char *name) {
	sqlite3 *db;
	int rv;

	if ((rv = sqlite3_open(name, &db)) != SQLITE_OK)
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	else
      fprintf(stderr, "Opened database successfully\n");
	
	// create table 
 	// mx_new_table_group_member(db); 
 	mx_new_table_user(db);
 	// mx_new_table_group(db); 
 	// mx_new_table_dialog(db);
	return db;
}

void mx_closedb(sqlite3 *db) {
	int val = sqlite3_close(db);

	if (val != SQLITE_OK) {
		fprintf(stderr, "Can't close database: %s\n", sqlite3_errmsg(db));
	}
	else 
		fprintf(stderr, "Closed database successfully\n");

}

static int callback(void *data, int argc, char **argv, char **azColName){
   fprintf(stderr, "%s: ", (const char*)data);
   for(int i = 0; i<argc; i++)
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   printf("\n");
   return 0;
}

void mx_print_db(sqlite3 *db, char *table) {
   char *sql = mx_strjoin("SELECT * from ", table);
   char *zErrMsg = 0;
   const char *data = table;
   int rc = 0;

   rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
}
