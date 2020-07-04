#include "client.h"

int mx_clear_jobj(json_object **jobj, int status) {
    free(*jobj);
    *jobj = NULL;

    return status;
}
