#include <stdio.h>
#include "uchat.h"

int main() {
   sqlite3 *db = mx_opendb("test.db");

   mx_insert_user_in_db(db, "admin1", "privet1111");
   mx_insert_user_in_db(db, "adm2", "poka2222");
   mx_insert_user_in_db(db, "adm3", "admin3333");
   

   // t_user *usr = mx_get_usertable(db);

   // printf("%d \t %s \t %s \n", usr->user_id, usr->user_login, usr->user_pass);
   // mx_printstr("\n");

   // t_user *next = mx_get_user_by_login(db, "log");

   mx_print_db(db, "USER");
   // printf("%d \t %s \t %s \n", next->user_id, next->user_login, next->user_pass);
   mx_printstr("===============================\n\n");

   mx_delete_user_by_login(db, "adm2");
   mx_print_db(db, "USER");

   mx_delete_user_by_id(db, 1);
   mx_printstr("===============================\n\n");
   
   mx_print_db(db, "USER");

   mx_closedb(db);

}

