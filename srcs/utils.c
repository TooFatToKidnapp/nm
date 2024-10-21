#include "../includes/nm.h"


uint64_t _strlen(const char * str) {
  if (str == NULL) {
    return 0;
  }
  uint64_t len;
  for (len = 0; str[len] ; ++len) {}
  return len;
}

uint64_t  _strlcat(char *dst, const char *src, uint64_t dstsize)
{
        uint64_t  i;
        uint64_t  n;
        uint64_t  dst_len;
        uint64_t  src_len;

        i = 0;
        n = 0;
        src_len = _strlen(src);
        if (!dst && src && dstsize == 0)
                return (src_len);
        dst_len = _strlen(dst);
        if (dstsize <= dst_len)
                return (dstsize + src_len);
        while (dst[n] != 0)
                n++;
        while ((dst_len + i) < (dstsize - 1) && src[i] != 0)
        {
                dst[n] = src[i];
                n++;
                i++;
        }
        dst[n] = '\0';
        return (dst_len + src_len);
}
