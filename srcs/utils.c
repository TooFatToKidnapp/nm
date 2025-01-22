#include "../includes/nm.h"

uint64_t _strlen(const char *str)
{
  if (str == NULL)
  {
    return 0;
  }
  uint64_t len;
  for (len = 0; str[len]; ++len)
  {
  }
  return len;
}

uint64_t _strlcat(char *dst, const char *src, uint64_t dstsize)
{
  uint64_t i;
  uint64_t n;
  uint64_t dst_len;
  uint64_t src_len;

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

int32_t _strncmp(const char *s1, const char *s2, uint32_t n)
{
  uint32_t i;

  i = 0;
  while (s1[i] && s2[i] && s1[i] == s2[i] && i < n)
    i++;
  if (i < n)
    return ((uint8_t)s1[i] - (uint8_t)s2[i]);
  else
    return (0);
}

int32_t _memcmp(const void *s1, const void *s2, uint64_t n)
{
  uint64_t i;
  const uint8_t *c_s1 = s1;
  const uint8_t *c_s2 = s2;

  i = 0;
  while (i < n)
  {
    if (c_s1[i] != c_s2[i])
      return (c_s1[i] - c_s2[i]);
    i++;
  }
  return (0);
}

uint8_t match_section_type(char* section_name, uint8_t type, uint32_t bind) {
  uint8_t c = 0;
  if (_strncmp(section_name, ".symtab", _strlen(".symtab")) == 0
    || _strncmp(section_name, ".strtab", _strlen(".strtab") == 0)
    || _strncmp(section_name, ".shstrtab", _strlen(".shstrtab")) == 0) {
      c = 'a';
    }
  if (_strncmp(section_name, ".gnu.hash", _strlen(".gnu.hash")) == 0
      || _strncmp(section_name, ".rodata", _strlen(".rodata")) == 0
      || _strncmp(section_name, ".dynstr", _strlen(".dynstr")) == 0
      || _strncmp(section_name, ".rela.plt", _strlen(".rela.plt")) == 0
      || _strncmp(section_name, ".rel.plt", _strlen(".rel.plt")) == 0
      || _strncmp(section_name, ".SUNW_version", _strlen(".SUNW_version")) == 0
      || _strncmp(section_name, ".rela.bss", _strlen(".rela.bss")) == 0
      || _strncmp(section_name, ".rela.got", _strlen(".rela.got")) == 0
      || _strncmp(section_name, ".gnu.version_r", _strlen(".gnu.version_r")) == 0)
    {
      c = 'R';
      if (bind == STB_LOCAL) c += 32;
    }
  if (_strncmp(section_name, ".debug_", _strlen(".debug_")) == 0) {
    c = 'N';
  }
  if (_strncmp(section_name, ".comment", _strlen(".comment")) == 0
      || _strncmp(section_name, ".copyright", _strlen(".copyright")) == 0
      || _strncmp(section_name, ".gnu_debug", _strlen(".gnu_debug")) == 0
      || _strncmp(section_name, ".ident", _strlen(".ident")) == 0
      || _strncmp(section_name, ".SUNW", _strlen(".SUNW")) == 0) {
        c = 'n';
      }
  if (c) return c;
  return type;
}
