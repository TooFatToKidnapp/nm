#include "../includes/nm.h"

void panic(const char * msg, int32_t status_code) {
  if (msg) {
    write(2, msg, _strlen(msg));
    write(2, "\n", 1);
  }
  if (status_code != -1) exit(status_code);
}

void clean_up(void * file_mapping, size_t file_size, int32_t fd) {
  if (fd != -1) {
    if (0 > close(fd)) {
      panic("Failed to close file fd", 1);
    }
  }
  if (file_mapping != NULL) {
    if (0 > munmap(file_mapping, file_size)) {
      panic("Failed to unmap file from memory", 1);
    }
  }

}
