#include "../includes/nm.h"

uint32_t get_32_bit_symbol_type(Elf32_Ehdr *ehdr, Elf32_Shdr* shdr, Elf32_Sym *symbol) {
  uint32_t bind = ELF32_ST_BIND(symbol->st_info);
  uint32_t type = ELF32_R_TYPE(symbol->st_info);
  uint16_t section_header_table_index = read_as_uint16_t(symbol->st_shndx);
  uint16_t section_header_intery_count = read_as_uint16_t(ehdr->e_shnum);
  uint32_t c;
  if (bind == STB_GNU_UNIQUE) {
    c = 'u';
  } else if (bind == STB_WEAK && type == STT_OBJECT) {
    if (section_header_table_index == SHN_UNDEF) c = 'v';
    else c = 'V';
  } else if (bind == STB_WEAK) {
    if (section_header_table_index == SHN_UNDEF) c = 'w';
    else c = 'W';
  } else if (section_header_table_index == SHN_UNDEF) c = 'U';
  else if (section_header_table_index == SHN_ABS) {
    c = 'A';
  } else if (section_header_table_index == SHN_COMMON) {
    c = 'C';
  } else if (section_header_table_index < section_header_intery_count) {
    uint32_t section_header_type = read_as_uint32_t(shdr[section_header_table_index].sh_type);
    uint32_t section_header_flags = read_as_uint32_t(shdr[section_header_table_index].sh_flags);
    if (section_header_type == SHT_NOBITS && section_header_flags == (SHF_ALLOC | SHF_WRITE))
      c = 'B';
    else if (section_header_flags == SHF_ALLOC &&
      (section_header_type == SHT_PROGBITS || section_header_type == SHT_RELA
      || section_header_type == SHT_REL || section_header_type == SHT_HASH
      || section_header_type == SHT_GNU_versym || section_header_type == SHT_GNU_verdef
      || section_header_type == SHT_STRTAB || section_header_type == SHT_DYNSYM
      || section_header_type == SHT_NOTE)) {
      c = 'R';
    } else if (section_header_type == SHT_PROGBITS && section_header_flags == (SHF_ALLOC | SHF_WRITE)) {
      c = 'D';
    } else if (section_header_type == SHT_PROGBITS && section_header_flags == (SHF_ALLOC | SHF_EXECINSTR)) {
      c = 'T';
    } else if (section_header_type == SHT_DYNAMIC) {
      c = 'D';
    } else c = ('t' - 32);
  }
  if (bind == STB_LOCAL && c != '?')
    c += 32;
  return c;
}
