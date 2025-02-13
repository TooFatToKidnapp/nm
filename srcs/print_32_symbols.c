#include "../includes/nm.h"

static void print_32_bit_symbol_table_entry(t_symbol* symbol) {
  uint16_t symbol_header_index = read_as_uint16_t(((Elf32_Sym*)symbol->symbol_ptr)->st_shndx);
  uint32_t value = read_as_uint32_t(((Elf32_Sym*)symbol->symbol_ptr)->st_value);
  if (symbol_header_index != SHN_UNDEF)
    // printf("%08x %c %s\n", value, symbol->type, symbol->name);
    print_symbol_details(value, symbol->type, symbol->name, true, true);
  else
    // printf("%s %1c %s\n", "        ", symbol->type, symbol->name);
    print_symbol_details(0, symbol->type, symbol->name, true, false);

}

void print_elf_32_symbols(t_list** head, e_cli_args* args) {
  uint64_t list_len = lst_len(*head);
  if (!is_arg_set(P, args)) {
    sort_lst(*head, sort_symbol_by_value_asc);
    if (!is_arg_set(R, args)) {
      sort_lst(*head, sort_symbol_asc);
    }
    else {
      sort_lst(*head, sort_symbol_dsc);
    }
  }
  list_len = lst_len(*head);
  for (uint64_t i = 0; i < list_len && *head; ++i) {
    t_list* node = get_symbol_at_index(*head, i, list_len);
    t_symbol* symbol = node->content;
    Elf32_Sym* symbol_ptr = symbol->symbol_ptr;
    uint16_t symbol_table_segment_header_entry = read_as_uint16_t(symbol_ptr->st_shndx);
    if (is_arg_set(U, args)) {
      if (symbol_table_segment_header_entry == SHN_UNDEF) {
        print_32_bit_symbol_table_entry(symbol);
      }
    } else if (is_arg_set(G, args)) {
      if (ELF32_ST_BIND(symbol_ptr->st_info) == STB_GLOBAL || ELF32_ST_BIND(symbol_ptr->st_info) == STB_WEAK)
        print_32_bit_symbol_table_entry(symbol);
    } else if (is_arg_set(A, args)) {
        print_32_bit_symbol_table_entry(symbol);
    } else {
      if (!(symbol_table_segment_header_entry == SHN_LOPROC || symbol_table_segment_header_entry == SHN_BEFORE || symbol_table_segment_header_entry == SHN_AFTER ||
            symbol_table_segment_header_entry == SHN_HIPROC || symbol_table_segment_header_entry == SHN_LOOS || symbol_table_segment_header_entry == SHN_HIOS ||
            symbol_table_segment_header_entry == SHN_ABS || symbol_table_segment_header_entry == SHN_COMMON || symbol_table_segment_header_entry == SHN_XINDEX ||
            symbol_table_segment_header_entry == SHN_HIRESERVE) && ELF32_ST_TYPE(symbol_ptr->st_info) != STT_SECTION)
        print_32_bit_symbol_table_entry(symbol);
    }
  }
}
