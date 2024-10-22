#include "../includes/nm.h"

void panic(const char * msg, uint32_t status_code) {
  if (msg) {
    write(2, msg, _strlen(msg));
    write(2, "\n", 1);
  }
  exit(status_code);
}
