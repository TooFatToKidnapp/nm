#include "../includes/nm.h"


void panic(const char * msg, uint32_t status_code) {
  if (msg) {
    perror(msg);
  }
  exit(status_code);
}
