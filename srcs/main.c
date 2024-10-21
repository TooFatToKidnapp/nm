#include "../includes/nm.h"

// ** the Executable and Linkable Format Resources **
// https://www.cs.cmu.edu/afs/cs/academic/class/15213-f00/docs/elf.pdf
// https://wiki.osdev.org/ELF
// https://en.wikipedia.org/wiki/Executable_and_Linkable_Format
// https://uclibc.org/docs/elf-64-gen.pdf
// https://linux-audit.com/elf-binaries-on-linux-understanding-and-analysis
// https://wiki.osdev.org/ELF_Tutorial
// https://en.wikipedia.org/wiki/Nm_(Unix)

int32_t main(int32_t ac, char **av)
{
  uint8_t * file_name = NULL;
  uint32_t file_fd = -1;
  if (2 > ac) {
    file_fd = get_file_handler(file_name);
    printf("-file_fd = %d\n", file_fd);

    if (0 > clone(file_fd)) {
      char buff[300] = {0};
      _strlcat(buff, "Failed to close file [", sizeof(buff));
      _strlcat(buff, (char *)file_name, sizeof(buff));
      _strlcat(buff, "]", sizeof(buff));
      panic(buff, 1);
    }
  } else {
    for (int32_t file_count = 1; file_count < ac ; ++file_count) {
      file_name = (uint8_t*)av[file_count];
      file_fd = get_file_handler(file_name);

      printf("--file_fd = %d\n", file_fd);
      if (0 > clone(file_fd)) {
        char buff[300] = {0};
        _strlcat(buff, "Failed to close file [", sizeof(buff));
        _strlcat(buff, (char *)file_name, sizeof(buff));
        _strlcat(buff, "]", sizeof(buff));
        panic(buff, 1);
      }
    }
  }

  return 0;
}
