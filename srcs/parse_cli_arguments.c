#include "../includes/nm.h"

uint32_t parse_and_count_cli_args(char ** arg_list, e_cli_args* args) {
  uint32_t file_count = 0;
  for(uint32_t i = 1; arg_list && arg_list[i]; ++i) {
    if (!_strncmp(arg_list[i], "-p\0", 3)) {
      *args |= ARG_P;
    } else if (!_strncmp(arg_list[i], "-a\0", 3)) {
      *args |= ARG_A;
    } else if (!_strncmp(arg_list[i], "-u\0", 3)) {
      *args |= ARG_U;
    } else if (!_strncmp(arg_list[i], "-g\0", 3)) {
      *args |= ARG_G;
    } else if (!_strncmp(arg_list[i], "-r\0", 3)) {
      *args |= ARG_R;
    } else if (arg_list[i][0] == '-') {
      char buff[300] = {0};
      _strlcat(buff, "Invalid Argument [", sizeof(buff));
      _strlcat(buff, arg_list[i], sizeof(buff));
      _strlcat(buff, "]", sizeof(buff));
      panic(buff, 1);
    } else {
      file_count++;
    }
  }
  return file_count;
}

int32_t get_file_handler(char *file_path) {
  if (file_path == NULL) {
    file_path = "./a.out";
  }
  int32_t fd = -1;
  fd = open(file_path, O_RDONLY);
  if (0 > fd) {
    char buff[300] = {0};
    _strlcat(buff, "Failed to open file [", sizeof(buff));
    _strlcat(buff, file_path, sizeof(buff));
    _strlcat(buff, "]", sizeof(buff));
    panic(buff, 1);
  }
  return fd;
}

bool is_arg_set(e_cli_args arg, e_cli_args * args) {
  if (arg & *args) {
    return true;
  }
  return false;
}

bool is_arg(char * arg) {
  return (
    !_strncmp(arg, "-p\0", 3)
      || !_strncmp(arg, "-u\0", 3)
        || !_strncmp(arg, "-a\0", 3)
          || !_strncmp(arg, "-g\0", 3)
            || !_strncmp(arg, "-r\0", 3)
  );
}
