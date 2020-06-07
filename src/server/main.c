#include <stdio.h>
#include "uchat.h"


static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   
   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   
   printf("\n");
   return 0;
}

int main() {
   sqlite3 *db = mx_opendb("test.db");

   mx_insert_user_in_db(db, "loh", "pidr");
   

   t_user *usr = mx_get_usertable(db);

   printf("%d \t %s \t %s \n", usr->user_id, usr->user_login, usr->user_pass);
   mx_printstr("\n");
   mx_insert_user_in_db(db, "log", "in");

   // t_user *next = mx_get_user_by_login(db, "log");
   char *sql = "SELECT * from USER";
   char *zErrMsg = 0;
   const char *data = "callback foo";

   int rc = 0;
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

   // printf("%d \t %s \t %s \n", next->user_id, next->user_login, next->user_pass);
// mx_printstr("suka");
   mx_closedb(db);

}

