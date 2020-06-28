#include "uchat.h"

// converts "str" to "\"str\""
char *mx_str_builder(char *str) {
    int len = mx_strlen(str);
    char *full_str = malloc(sizeof(char) * (len + 3));
    full_str[0] = '"';
    mx_strcpy(&full_str[1], str);
    full_str[len + 1] = '"';
    full_str[len + 2] = '\0';

    return full_str;
}

char *mx_array_to_str(int *arr, int len) {
    char *str = mx_strdup("[");
    char *tmp = NULL;
    char *itoa = NULL;

    for (int i = 0; i < len; i++) {
        itoa = mx_itoa(arr[i]);
        tmp = mx_strjoin(str, itoa);
        mx_strdel(&str);
        mx_strdel(&itoa);
        str = tmp;
        if (i != len - 1) {
            tmp = mx_strjoin(str, ",");
            mx_strdel(&str);
            str = tmp;
        }
    }

    tmp = mx_strjoin(str, "]");
    mx_strdel(&str);
    return tmp;
}

char *mx_json_builder(int num, ...) {
    char *str = mx_strdup("{");
    char *tmp = NULL;
    va_list args;

    va_start(args, num);
    for (int i = 0; i < num; i++) {
        tmp = mx_strjoin(str, va_arg(args, char *));
        mx_strdel(&str);
        str = tmp;
        if (i % 2 == 1 && i != num - 1) {
            tmp = mx_strjoin(str, ",");
            mx_strdel(&str);
            str = tmp;
        }
    }
    va_end(args);
    tmp = mx_strjoin(str, "}");
    mx_strdel(&str);

    return tmp;
}

// // json_builder how-to
// int main() {
//     char *str = mx_json_builder(16, "\"type\":", "\"S_MES\"", "\"gid\":", "-1",
//                                 "\"did\":", "-2", "\"uid\":", "2", "\"uid2\":",
//                                 "1", "\"msg\":", "\"Hello I'm Geralt\"",
//                                 "\"time\":", "3819524", "\"file\":", "\"\"");
//     char *str2 = mx_json_builder(6, "\"id\":", "1", "\"num\":", "2", "\"file\":",
//                                  "[1, 2, 3, 4, 5, 6, 1]");

//     printf("%s \n%s\n", str, str2);
// }

// // output:
// {"type":"S_MES","gid":-1,"did":-2,"uid":2,"uid2":1,"msg":"Hello I'm Geralt","time":3819524,"file":""}
// {"id":1,"num":2,"file":[1, 2, 3, 4, 5, 6, 1]}
