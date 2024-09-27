#include "../includes/nm.h"

// ** the Executable and Linkable Format Resources **
// https://www.cs.cmu.edu/afs/cs/academic/class/15213-f00/docs/elf.pdf
// https://wiki.osdev.org/ELF
// https://en.wikipedia.org/wiki/Executable_and_Linkable_Format
// https://uclibc.org/docs/elf-64-gen.pdf

int32_t main(int32_t ac, char **av)
{
  (void)ac;
  (void)av;
  printf("Hello World\n");

  return 0;
}
