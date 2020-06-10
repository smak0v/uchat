#include "libmx.h"

static void free_ptr(void *ptr) {
    free(ptr);
    ptr = NULL;
}

static size_t get_size_to_copy(void *p, void *np) {
    return malloc_size(p) < malloc_size(np) ? malloc_size(p) : malloc_size(np);
}

static void copy_data(void *ptr, void *new_ptr) {
    size_t size_to_copy = get_size_to_copy(ptr, new_ptr);

    new_ptr = mx_memcpy(new_ptr, ptr, size_to_copy);
    free_ptr(ptr);
}

void *mx_realloc(void *ptr, size_t size) {
    void *result = NULL;

    if (((!size) && (ptr)) || ((!size) && (!ptr))) {
        free_ptr(ptr);
        return malloc(0);
    }
    if ((size) && (!ptr))
        return malloc(size);
    result = malloc(size);
    if (!result)
        return NULL;
    if (malloc_size(ptr) < malloc_size(result))
        copy_data(ptr, result);
    else {
        if (malloc_size(ptr) < size - 32)
            copy_data(ptr, result);
        else
            result = ptr;
    }
    return result;
}
