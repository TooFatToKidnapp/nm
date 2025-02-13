#include "../includes/nm.h"

static bool has_symbols_64(t_elf_file *file_info, Elf64_Ehdr *ehdr, Elf64_Shdr *shdr)
{
  uint16_t e_shnum = read_as_uint16_t(ehdr->e_shnum);
  uint16_t e_shstrndx = read_as_uint16_t(ehdr->e_shstrndx);
  if (e_shstrndx == SHN_XINDEX)
  {
    e_shstrndx = read_as_uint32_t(shdr[0].sh_link);
  }
  const char *shstrtab = file_info->file_content + read_as_uint64_t(shdr[e_shstrndx].sh_offset);
  for (int i = 0; i < e_shnum; ++i)
  {
    uint32_t sect_type = read_as_uint32_t(shdr[i].sh_type);
    const char *section_name = shstrtab + read_as_uint32_t(shdr[i].sh_name);
    if (sect_type == SHT_SYMTAB || sect_type == SHT_DYNSYM)
    {
      if (sect_type == SHT_SYMTAB && _strncmp(section_name, ".symtab", 8) == 0)
      {
        uint32_t entsize = read_as_uint64_t(shdr[i].sh_entsize);
        if (entsize == 0)
          continue;
        uint32_t num_symbols = read_as_uint64_t(shdr[i].sh_size) / entsize;
        if (num_symbols > 1)
        {
          return true;
        }
      }
    }
  }
  return false;
}

static int32_t parse64_section_table(t_elf_file *file_info, e_cli_args *args,
              uint32_t symtab_index, Elf64_Shdr *section_header, Elf64_Ehdr *elf_header)
{

  uint32_t string_table_index = read_as_uint32_t(section_header[symtab_index].sh_link);
  uint16_t section_header_string_index = read_as_uint16_t(elf_header->e_shstrndx);
  uint16_t section_header_count = read_as_uint16_t(elf_header->e_shnum);
  uint64_t strtab_sh_offset = read_as_uint64_t(section_header[string_table_index].sh_offset);
  uint64_t strtab_sh_size = read_as_uint64_t(section_header[string_table_index].sh_size);
  uint64_t symtab_sh_offset = read_as_uint64_t(section_header[symtab_index].sh_offset);
  uint64_t symtab_sh_size = read_as_uint64_t(section_header[symtab_index].sh_size);
  uint64_t symtab_sh_enty_size = read_as_uint64_t(section_header[symtab_index].sh_entsize);
  uint64_t shstr_sh_offset = read_as_uint64_t(section_header[section_header_string_index].sh_offset);
  uint64_t shstr_sh_size = read_as_uint64_t(section_header[section_header_string_index].sh_size);
  if (sizeof(Elf64_Sym) != symtab_sh_enty_size
      || string_table_index >= section_header_count
      || section_header_string_index >= section_header_count
      || file_info->file_size < strtab_sh_offset + strtab_sh_size
      || file_info->file_size < symtab_sh_offset + symtab_sh_size
      || file_info->file_size < shstr_sh_offset + shstr_sh_size)
  {
    panic("Invalid Symbol table", -1);
    return 1;
  }

  char *string_table_ptr = (char *)(file_info->file_content + strtab_sh_offset);
  char *sh_string_table_ptr = (char *)(file_info->file_content + shstr_sh_offset);
  Elf64_Sym *sym_table = (Elf64_Sym *)(file_info->file_content + symtab_sh_offset);
  uint32_t sym_table_len = symtab_sh_size / sizeof(Elf64_Sym);
  t_list *symbol_lst = NULL;
  for (uint32_t i = 1; i < sym_table_len; ++i)
  {
    uint32_t symtab_st_name = read_as_uint32_t(sym_table[i].st_name);
    char *name = string_table_ptr + symtab_st_name;
    uint8_t type = get_64_bit_symbol_type(elf_header, section_header, &sym_table[i]);
    uint16_t st_shndx = read_as_uint16_t(sym_table[i].st_shndx);
    if (st_shndx < section_header_count)
    {
      uint32_t shstrtabidx = read_as_uint32_t(section_header[st_shndx].sh_name);
      if (ELF64_ST_TYPE(sym_table[i].st_info) == STT_SECTION)
      {
        name = sh_string_table_ptr + shstrtabidx;
      }
    }
    type = match_section_type(name, type, ELF64_ST_BIND(sym_table[i].st_info));
    push_back_node(&symbol_lst, format_symbol(name, type, read_as_uint64_t(sym_table[i].st_value), sym_table + i, st_shndx));
  }
  print_elf_64_symbols(&symbol_lst, args);
  clear_lst(&symbol_lst);
  return 0;
}

int32_t nm64(t_elf_file *file_info, e_cli_args *args)
{
  if (file_info->file_size < sizeof(Elf64_Ehdr))
  {
    panic("File too small", -1);
    return 1;
  }

  Elf64_Ehdr *ehdr = (Elf64_Ehdr *)file_info->file_content;
  uint32_t e_shoff = read_as_uint64_t(ehdr->e_shoff);
  if (e_shoff > file_info->file_size)
  {
    panic("Invalid e_shoff value", -1);
    return 1;
  }

  Elf64_Shdr *section_header_ptr = (Elf64_Shdr *)(file_info->file_content + e_shoff);
  if (has_symbols_64(file_info, ehdr, section_header_ptr) == false)
  {
    DBG("ft_nm: %s: no symbols\n", file_info->file_name);
    return 0;
  }

  uint16_t section_header_table_index = read_as_uint16_t(ehdr->e_shstrndx);
  uint32_t section_header_type = read_as_uint32_t(section_header_ptr[section_header_table_index].sh_type);
  uint16_t section_header_count = read_as_uint16_t(ehdr->e_shnum);
  if (section_header_table_index >= section_header_count || section_header_type != SHT_STRTAB)
  {
    panic("Invalid section table header", -1);
    return 1;
  }
  uint32_t section_header_size = read_as_uint64_t(section_header_ptr[0].sh_size);
  uint32_t section_header_offset = read_as_uint64_t(section_header_ptr[0].sh_offset);
  if (section_header_size != 0 && section_header_offset != 0)
  {
    panic("Invalid section table", -1);
    return 1;
  }

  if (section_header_count == 0)
  {
    panic("Not symbol table found", -1);
    return 1;
  }

  for (uint32_t i = 0; i < section_header_count; ++i)
  {
    uint32_t section_header_name = read_as_uint32_t(section_header_ptr[i].sh_name);
    section_header_size = read_as_uint64_t(section_header_ptr[section_header_table_index].sh_size);
    section_header_type = read_as_uint32_t(section_header_ptr[i].sh_type);
    if (section_header_name > section_header_size)
    {
      panic("Invalid section table header", -1);
      return 1;
    }
    if (section_header_type == SHT_SYMTAB)
    {
      return parse64_section_table(file_info, args, i, section_header_ptr, ehdr);
    }
  }
  return 0;
}
