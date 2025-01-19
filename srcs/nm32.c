#include "../includes/nm.h"

int32_t parse32_section_table(t_elf_file* file_info, e_cli_args *args,
    uint32_t syntab_index, Elf32_Shdr* section_header ,Elf32_Ehdr* elf_header) {

  uint32_t string_table_index = read_as_uint32_t(section_header[syntab_index].sh_link);
  uint16_t section_header_string_index = read_as_uint16_t(elf_header->e_shstrndx);
  uint16_t section_header_count = read_as_uint16_t(elf_header->e_shnum);
  uint32_t strtab_sh_offset = read_as_uint32_t(section_header[string_table_index].sh_offset);
  uint32_t strtab_sh_size = read_as_uint32_t(section_header[string_table_index].sh_size);
  uint32_t symtab_sh_offset = read_as_uint32_t(section_header[syntab_index].sh_offset);
  uint32_t symtab_sh_size = read_as_uint32_t(section_header[syntab_index].sh_size);
  uint32_t symtab_sh_enty_size = read_as_uint32_t(section_header[syntab_index].sh_entsize);
  uint32_t shstr_sh_offset = read_as_uint32_t(section_header[section_header_string_index].sh_offset);
  uint32_t shstr_sh_size = read_as_uint32_t(section_header[section_header_string_index].sh_size);
  printf("sizeof(Elf32_Sym) = %ld | symtab_sh_enty_size = %d\n", sizeof(Elf32_Sym), symtab_sh_enty_size);
  printf("string_table_index = %u | section_header_count = %u\n", string_table_index, section_header_count);
  printf("section_header_string_index = %u | section_header_count = %u\n", section_header_string_index, section_header_count);
  printf("file_info->file_size = %lu | strtab_sh_offset + strtab_sh_size = %u\n", file_info->file_size, strtab_sh_offset + strtab_sh_size);
  printf("file_info->file_size = %lu | symtab_sh_offset + symtab_sh_size = %u\n", file_info->file_size, symtab_sh_offset + symtab_sh_size);
  printf("file_info->file_size = %lu | shstr_sh_offset + shstr_sh_size = %u\n", file_info->file_size, shstr_sh_offset + shstr_sh_size);
  if (sizeof(Elf32_Sym) != symtab_sh_enty_size
    || string_table_index >= section_header_count
    || section_header_string_index >= section_header_count
    || file_info->file_size < strtab_sh_offset + strtab_sh_size
    || file_info->file_size < symtab_sh_offset + symtab_sh_size
    || file_info->file_size < shstr_sh_offset + shstr_sh_size) {
      panic("Invalid Symbol table", -1);
      return 1;
    }

  (void)args;

  return 0;
}


int32_t nm32(t_elf_file* file_info, e_cli_args* args) {
  (void)args;
  if (file_info->file_size < sizeof(Elf32_Ehdr)) {
    panic("File too small", -1);
    return 1;
  }

  Elf32_Ehdr *ehdr = (Elf32_Ehdr*)file_info->file_content;
  uint32_t e_shoff = read_as_uint32_t(ehdr->e_shoff);
  if (e_shoff > file_info->file_size) {
    panic("Invalid e_shoff value", -1);
    return 1;
  }

  Elf32_Shdr *section_header_ptr = (Elf32_Shdr*)(file_info->file_content + e_shoff);
  uint16_t section_header_table_index = read_as_uint16_t(ehdr->e_shstrndx);
  uint32_t section_header_type = read_as_uint32_t(section_header_ptr[section_header_table_index].sh_type);
  uint16_t section_header_count = read_as_uint16_t(ehdr->e_shnum);
  if (section_header_table_index >= section_header_count || section_header_type != SHT_STRTAB) {
    panic("Invalid section table header", -1);
    return 1;
  }
  uint32_t section_header_size = read_as_uint32_t(section_header_ptr[0].sh_size);
  uint32_t section_header_offset = read_as_uint32_t(section_header_ptr[0].sh_offset);
  if (section_header_size != 0 && section_header_offset != 0) {
    panic("Invalid section table", -1);
    return 1;
  }

  if (section_header_count == 0) {
    panic("Not symbol table found", -1);
    return 1;
  }

  for (uint32_t i = 0; i < section_header_count; ++i) {
    uint32_t section_header_name = read_as_uint32_t(section_header_ptr[i].sh_name);
    section_header_size = read_as_uint32_t(section_header_ptr[section_header_table_index].sh_size);
    section_header_type = read_as_uint32_t(section_header_ptr[i].sh_type);
    if (section_header_name > section_header_size) {
      panic("Invalid section table header", -1);
      return 1;
    }
    if (section_header_type == SHT_SYMTAB) {
      return parse32_section_table(file_info, args, i, section_header_ptr, ehdr);
    }
  }
  return 0;
}
