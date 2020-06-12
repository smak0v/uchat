#include <stdio.h>
#include "uchat.h"

int main() {
   sqlite3 *db = mx_opendb("test.db");

   mx_add_user(db, "Login", "Password");

   mx_print_db(db, "USER");
   // mx_change_user_pass(db, 1, "parol2.0");
   mx_add_group_member(db, 1, 2);
   mx_add_group_member(db, 23123, 12312321);
   // mx_printint(d->dialog_id);
   // mx_printint(d->user_id1);
   // mx_printint(d->user_id2);
   mx_print_db(db, "GROUP_MEMBERS");
   mx_delete_user_from_group(db, 23123, 12312321);
   mx_print_db(db, "GROUP_MEMBERS");

   mx_closedb(db);

}

