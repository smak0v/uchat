#include "client.h"

static t_profile *build_profile(t_glade *g) {
    t_profile *profile = mx_memalloc(sizeof(t_profile));

    profile->country = (char *)gtk_entry_get_text(GTK_ENTRY(g->e_country));
    profile->dob = (char *)gtk_entry_get_text(GTK_ENTRY(g->e_birthday));
    profile->email = (char *)gtk_entry_get_text(GTK_ENTRY(g->e_email));
    profile->name = (char *)gtk_entry_get_text(GTK_ENTRY(g->e_profile_name));
    profile->status = (char *)gtk_entry_get_text(GTK_ENTRY(g->e_status));

    return profile;
}

void mx_save_profile(GtkWidget *w, t_glade *g) {
    t_profile *profile = build_profile(g);
    char *request = mx_json_string_edit_profile(g, profile);

    SSL_write(g->ssl, request, strlen(request));

    free(profile);
    profile = NULL;

    mx_strdel(&request);

    (void)w;
}
