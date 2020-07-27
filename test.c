#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    DIR *dir = opendir(argv[1]);
    if (dir) {
        closedir(dir);
    }
    else if (ENOENT == errno) {
        mkdir(argv[1], 0700);
	}
}

