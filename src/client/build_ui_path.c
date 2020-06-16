#include "uchat.h"

char *mx_build_ui_path(char *filename) {
    char *path = mx_strnew(mx_strlen(MX_UI_PATH) + mx_strlen(filename));

    path = mx_strcpy(path, MX_UI_PATH);
    path += mx_strlen(MX_UI_PATH);
    path = mx_strcpy(path, filename);
    path -= mx_strlen(MX_UI_PATH);

    return path;
}
