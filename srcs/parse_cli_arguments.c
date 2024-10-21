#include "../includes/nm.h"

uint32_t get_file_handler(uint8_t *file_path) {
  if (file_path == NULL) {
    file_path = (uint8_t *)"./a.out";
  }
  int32_t fd = -1;
  fd = open((char *)file_path, O_RDONLY | O_SYMLINK);
  if (0 > fd) {
    char buff[300] = {0};
    _strlcat(buff, "Failed to open file [", sizeof(buff));
    _strlcat(buff, (char *)file_path, sizeof(buff));
    _strlcat(buff, "]", sizeof(buff));
    panic(buff, 1);
  }
  return fd;
}
