#include "uchat.h"

int mx_init_client(int argc, char **argv) {
   GtkBuilder *builder;
   GObject *window;
   GError *error = NULL;
   char *ui_path = mx_build_ui_path("builder.ui");

   gtk_init(&argc, &argv);
   builder = gtk_builder_new();
   if (gtk_builder_add_from_file(builder, ui_path, &error) == 0) {
      g_printerr("Error loading file: %s\n", error->message);
      g_clear_error(&error);
      return MX_FAILURE;
   }

   window = gtk_builder_get_object(builder, "window");
   g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

   gtk_main();
   mx_strdel(&ui_path);

   return MX_SUCCESS;
}
