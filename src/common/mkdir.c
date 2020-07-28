#include "uchat.h"

void mx_mkdir(char *filename) {
    DIR *dir = opendir(filename);

    if (dir) {
        closedir(dir);
    }
    else if (ENOENT == errno) {
        mkdir(filename, 0700);
    }
}
