#include "../includes/nm.h"


uint32_t get_32_bit_symbol_type(Elf32_Ehdr *ehdr, Elf32_Shdr* shdr, const Elf32_Sym *symbol) {
  uint32_t bind = ELF32_ST_BIND(symbol->st_info);
  uint32_t type = ELF32_ST_TYPE(symbol->st_info);
  uint16_t section_index = read_as_uint16_t(symbol->st_shndx);
  uint16_t shnum = read_as_uint16_t(ehdr->e_shnum);
  uint32_t c = '?';

  if (bind == STB_GNU_UNIQUE) {
    c = 'u';
  } else if (bind == STB_WEAK) {
    if (type == STT_OBJECT) {
        c = (section_index == SHN_UNDEF) ? 'v' : 'V';
    } else {
      c = (section_index == SHN_UNDEF) ? 'w' : 'W';
    }
  } else if (section_index == SHN_UNDEF) {
    c = 'U';
  } else if (section_index == SHN_ABS) {
    c = 'A';
  } else if (section_index == SHN_COMMON) {
    c = 'C';
  } else if (section_index < shnum) {
    Elf32_Shdr *section = &shdr[section_index];
    uint32_t sect_type = read_as_uint32_t(section->sh_type);
    uint32_t sect_flags = read_as_uint32_t(section->sh_flags);
    if (sect_type == SHT_NOBITS && (sect_flags & (SHF_ALLOC | SHF_WRITE)) == (SHF_ALLOC | SHF_WRITE)) {
      c = 'B';  // BSS (uninitialized data)
    } else if (sect_type == SHT_PROGBITS) {
      if ((sect_flags & (SHF_ALLOC | SHF_WRITE)) == (SHF_ALLOC | SHF_WRITE)) {
        c = 'D';  // Writable data (e.g., .data)
      } else if ((sect_flags & (SHF_ALLOC | SHF_EXECINSTR)) == (SHF_ALLOC | SHF_EXECINSTR)) {
        c = 'T';  // Executable code (e.g., .text)
      } else if ((sect_flags & SHF_ALLOC) == SHF_ALLOC) {
        c = 'R';  // Read-only data (e.g., .rodata)
      }
    } else if (sect_type == SHT_INIT_ARRAY || sect_type == SHT_FINI_ARRAY || sect_type == SHT_PREINIT_ARRAY) {
      c = 'D';  // Initialization arrays (e.g., .init_array)
    } else if (sect_type == SHT_DYNAMIC) {
      c = 'D';  // Dynamic linking section (e.g., .dynamic)
    } else if ((sect_flags & SHF_ALLOC) == SHF_ALLOC) {
      c = 'R';  // Default for allocated sections
    } else {
      c = 'T';  // Fallback to text
    }
  }

  if (bind == STB_LOCAL && c != '?') {
    c += 32;
  }

  return c;
}
