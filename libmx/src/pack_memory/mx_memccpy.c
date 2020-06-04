#include "libmx.h"

void *mx_memccpy(void *restrict dst, const void *restrict src, int c,
                 size_t n) {
     size_t i = 0;
     unsigned char *p_dst = (unsigned char *)dst;
     unsigned char *p_src = (unsigned char *)src;

     while (i < n) {
         p_dst[i] = p_src[i];
         if (p_src[i] == (unsigned char)c)
             return (p_dst + (++i));
         i++;
     }
     return NULL;
}
