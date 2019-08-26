#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

void *minic_malloc(const size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "out of memory");
        exit(EXIT_FAILURE);
    } else {
        return ptr;
    }
}

char *make_str(const char *str) {
    const size_t str_len = strlen(str);
    char *dst = minic_malloc(str_len + 1);
    memcpy(dst, str, str_len);
    return dst;
}
