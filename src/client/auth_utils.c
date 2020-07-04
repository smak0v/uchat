#include "client.h"

// {"type": "REG", "name": "kali", "passw": "qwerty"}
// {"type": "REG", "name": "geralt", "passw": "qwerty"}
// {"type": "REG", "name": "BogdanUeban", "passw": "qwerty"}
// {"type": "REG", "name": "MrSkout", "passw": "qwerty"}
// {"type": "S_IN", "name": "kali", "passw": "qwerty"}
// {"type": "S_IN", "name": "geralt", "passw": "qwerty"}
// {"type": "S_IN", "name": "BogdanUeban", "passw": "qwerty"}
// {"type": "S_IN", "name": "MrSkout", "passw": "qwerty"}
// {"type": "S_MES", "gid": -1, "did": -2, "uid": 2, "uid2": 1, "msg": "Hello I'm Geralt", "time": 3819524, "file": ""}
// {"type": "S_OUT", "id": 1}
// {"type": "S_OUT", "id": 2}

// {"type": "N_GRP", "name": "TEST2", "id": [1], "tok":"MKK;KPK]KaKKKKK'B-KKK:XFKpDuK+KKOKKKKKKIS:O\/`KKKKXKKKKKX?KgKMzKKKK_<L(KKKCKKKKkKKKK,KPe?KK6KqKK(<K2KKHKKdKKKKaKKZKKKKlKQKNKJKK7KKKK$yKKrKKJ,K2KKKK!KKKKKKKKKK-KKKKK!KKJK:KKFKyK|(KyKHK1KKKWKKKKSKv<0KKgUKtKKKKKKVKDH&K`KKKKK7JKK)gKKKK\/KKeKoK]KK>KKKKKKqKrsK?fLK"}
// {"type": "INV", "gid": 1, "add": [2], "uid": 1, "tok": "MKK;KPK]KaKKKKK'B-KKK:XFKpDuK+KKOKKKKKKIS:O\/`KKKKXKKKKKX?KgKMzKKKK_<L(KKKCKKKKkKKKK,KPe?KK6KqKK(<K2KKHKKdKKKKaKKZKKKKlKQKNKJKK7KKKK$yKKrKKJ,K2KKKK!KKKKKKKKKK-KKKKK!KKJK:KKFKyK|(KyKHK1KKKWKKKKSKv<0KKgUKtKKKKKKVKDH&K`KKKKK7JKK)gKKKK\/KKeKoK]KK>KKKKKKqKrsK?fLK"}
// // {"type": "S_MES", "gid": 1, "did": -1, "uid": 1, "uid2": -1, "msg": "привет", "time": 3819524, "file": "", "tok":"MKK;KPK]KaKKKKK'B-KKK:XFKpDuK+KKOKKKKKKIS:O\/`KKKKXKKKKKX?KgKMzKKKK_<L(KKKCKKKKkKKKK,KPe?KK6KqKK(<K2KKHKKdKKKKaKKZKKKKlKQKNKJKK7KKKK$yKKrKKJ,K2KKKK!KKKKKKKKKK-KKKKK!KKJK:KKFKyK|(KyKHK1KKKWKKKKSKv<0KKgUKtKKKKKKVKDH&K`KKKKK7JKK)gKKKK\/KKeKoK]KK>KKKKKKqKrsK?fLK"}
// {"type": "S_MES", "gid": -1, "did": -2, "uid": 1, "uid2": 2, "msg": "привет", "time": 3819524, "file": "", "tok":"MKK;KPK]KaKKKKK'B-KKK:XFKpDuK+KKOKKKKKKIS:O\/`KKKKXKKKKKX?KgKMzKKKK_<L(KKKCKKKKkKKKK,KPe?KK6KqKK(<K2KKHKKdKKKKaKKZKKKKlKQKNKJKK7KKKK$yKKrKKJ,K2KKKK!KKKKKKKKKK-KKKKK!KKJK:KKFKyK|(KyKHK1KKKWKKKKSKv<0KKgUKtKKKKKKVKDH&K`KKKKK7JKK)gKKKK\/KKeKoK]KK>KKKKKKqKrsK?fLK"}

// "MKK;KPK]KaKKKKK'B-KKK:XFKpDuK+KKOKKKKKKIS:O\/`KKKKXKKKKKX?KgKMzKKKK_<L(KKKCKKKKkKKKK,KPe?KK6KqKK(<K2KKHKKdKKKKaKKZKKKKlKQKNKJKK7KKKK$yKKrKKJ,K2KKKK!KKKKKKKKKK-KKKKK!KKJK:KKFKyK|(KyKHK1KKKWKKKKSKv<0KKgUKtKKKKKKVKDH&K`KKKKK7JKK)gKKKK\/KKeKoK]KK>KKKKKKqKrsK?fLK"
// "KKKKKC5FKKMKKK(KKKKKKghKKK7KKKJKPKKKiKv^IKK&K,KSKKKKKK?KK;KKKKKhtKlsKKKKKKKKKKKKKKK2Kc]KKJKGhK4K@K]KiwKK]\/XKKKKKKK\\*KBKYKKKqKKKK?BlKKKKKKKeKH:KK-:KUMKoKKK`KKKKKKKIKKKcKP\\zKKKKKKK4KKKuKKKKKKKd;K~KqK@KKiNKKKKK|K'9KKB*K@KyKKKK7toKKQQKKKvGKKKKKKKKKKKKKKKKKKKKM"

void mx_open_regwin(GtkWidget *sender, t_glade *g) {
    int w = 0;
    int h = 0;
    int x = 0;
    int y = 0;

    (void)sender;
    gtk_window_get_position(GTK_WINDOW(g->w_log), &x, &y);
    gtk_window_get_size(GTK_WINDOW(g->w_log), &w, &h);
    gtk_widget_hide(g->w_log);

    gtk_window_resize(GTK_WINDOW(g->w_reg), w, h);
    gtk_window_move(GTK_WINDOW(g->w_reg), x, y + 22.35);
    gtk_widget_show_all(GTK_WIDGET(g->w_reg));

    mx_widget_set_visibility(g->bd, "r_pass_err", FALSE);
    mx_widget_set_visibility(g->bd, "r_pass_err1", FALSE);
    mx_widget_set_visibility(g->bd, "r_name_err", FALSE);
}

void mx_b_reg_log(GtkButton *b, t_glade *g) {
    char *repeat = (char *)gtk_entry_get_text(GTK_ENTRY(g->r_repass));
    char *request = NULL;
    char *response = NULL;

    (void)b;
    g->log = (char *)gtk_entry_get_text(GTK_ENTRY(g->r_ename));
    g->pass = (char *)gtk_entry_get_text(GTK_ENTRY(g->r_epass));

    if (!mx_validate_signup_data(g, repeat)) {
        request = mx_json_string_login_signup(REG, g->log, g->pass);
        SSL_write(g->ssl, request, strlen(request));
        response = mx_read_server_response(g);
        mx_printstr_endl(response);
        mx_strdel(&request);
        mx_strdel(&response);
        // mx_open_win_chat(g->w_reg, g);
    }
}

 void mx_b_log(GtkButton *b, t_glade *g) {
    char *request = NULL;
    char *response = NULL;

    g->log = (char *)gtk_entry_get_text(GTK_ENTRY(g->e_name));
    g->pass = (char *)gtk_entry_get_text(GTK_ENTRY(g->e_pass));

    (void)b;
    if (!mx_validate_login_data(g)) {
        request = mx_json_string_login_signup(S_IN, g->log, g->pass);
        SSL_write(g->ssl, request, strlen(request));
        response = mx_read_server_response(g);
        if (!mx_parse_login_response(response, g))
            mx_open_win_chat(g->w_log, g);
        mx_strdel(&request);
        mx_strdel(&response);
    }
}
