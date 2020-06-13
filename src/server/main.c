#include <stdio.h>
#include "uchat.h"

int main() {
   sqlite3 *db = mx_opendb("test.db");

   int i = 0;

   // i = mx_add_dialog(db, 3, 3);
   // i = mx_add_dialog(db, 3, 1);
   i = mx_add_user(db, "login", "pass");

   // printf("\t first:%d\n", i);
   i = mx_add_user(db, "name", "parol");
   // printf("\t second:%d\n", i);
   i = mx_add_user(db, "name23", "pass");

   mx_print_db(db, "USER");
   t_user *u = mx_get_user_by_login(db, "name223");
   // t_user *u = mx_get_user_by_user_id(db, 3);
   // printf("***** login:%s\n", u->user_login);
      // mx_printstr_endl(u->user_login);

   if (u)
      mx_printstr_endl(u->user_login);
   else
      mx_printstr_endl("not found");

   // printf("***** login:%s\n", b->user_login);





   mx_closedb(db);

}

