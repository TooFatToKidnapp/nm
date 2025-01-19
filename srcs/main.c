#include "../includes/nm.h"

// ** the Executable and Linkable Format Resources **
// https://www.cs.cmu.edu/afs/cs/academic/class/15213-f00/docs/elf.pdf
// https://wiki.osdev.org/ELF
// https://en.wikipedia.org/wiki/Executable_and_Linkable_Format
// https://uclibc.org/docs/elf-64-gen.pdf
// https://linux-audit.com/elf-binaries-on-linux-understanding-and-analysis
// https://wiki.osdev.org/ELF_Tutorial
// https://en.wikipedia.org/wiki/Nm_(Unix)

/*
  ** compile into big endian format
  ** sudo apt-get install gcc-powerpc-linux-gnu
  ** powerpc-linux-gnu-gcc -m32 -o 32_bit_big_endan main.c -I../includes
*/

int32_t main(int32_t ac, char **av)
{
  e_cli_args args = {0};

  uint32_t file_count = parse_and_count_cli_args(av, &args);
  if (file_count == 0) {
    nm(NULL, &args);
  } else {
    for (int32_t i = 1; i < ac; ++i) {
      if (is_arg(av[i]) == false) {
        nm(av[i], &args);
      }
    }
  }
  return 0;
}
