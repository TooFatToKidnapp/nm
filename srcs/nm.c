#include "../includes/nm.h"

void nm(char * file_path, e_cli_args* args) {
  printf("file path = [%s]\n", file_path);
  int32_t fd = get_file_handler(file_path);
  (void)fd;
  (void)args;

}
