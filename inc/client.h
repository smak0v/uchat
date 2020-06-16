#pragma once

// Includes
#include <gtk/gtk.h>

// Constants
#define MX_UI_PATH "./src/client/ui/"


// Structures


// Functions
int mx_init_client(int argc, char **argv);

// UI
char *mx_build_ui_path(char *filename);
