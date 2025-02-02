#include "../includes/nm.h"

void clear_lst(t_list **head) {
  if (*head != NULL) {
    t_list *ptr;
    for (ptr = *head; ptr && ptr->next;) {
      t_list* tmp = ptr->next;
      free(ptr->content);
      free(ptr);
      ptr = tmp;
    }
      free(ptr->content);
      free(ptr);
  }
}

t_list *build_node(t_symbol *content) {
  if (content == NULL) {
    panic("Empty node content", 1);
  }
  t_list * new = malloc(sizeof(t_list));
  if (NULL == new) {
    panic("Failed to allocate node" , 1);
  }
  new->next = NULL;
  new->prev = NULL;
  new->content = content;
  return new;
}


void push_back_node(t_list **head, t_symbol *content) {
  if (*head != NULL) {
    t_list* ptr = NULL;
    for(ptr = *head; ptr != NULL && ptr->next != NULL; ptr = ptr->next);
    t_list *new = build_node(content);
    ptr->next = new;
    new->prev = ptr;
  } else {
    *head = build_node(content);
  }
}

void print_lst(t_list **head) {
    if (*head != NULL) {
    t_list* ptr = *head;
    for (uint32_t i = 0; ptr != NULL && ptr->next != NULL; ptr = ptr->next ) {
      printf("node pos = %d | node adder = %p |content adder = %p\n", i++, (void*)ptr, (void*)(ptr->content));
    }
    return;
  }
  printf("========LST IS EMPTY=========\n");
}

t_symbol *format_symbol(char *name, uint8_t type, uint64_t value, Elf32_Sym *ptr, uint16_t index) {
  if (!name || !ptr) panic("can't format symbol", 1);
  t_symbol * s = malloc(sizeof(t_symbol));
  if (!s) panic("Failed to allocate memory for symbol", 1);
  s->name = name;
  s->type = type;
  s->value = value;
  s->symbol_ptr = ptr;
  s->segment_header_index = index;
  return s;
}

t_list * get_symbol_at_index(t_list* head, uint64_t index, uint64_t list_len) {
  if (!head || index > list_len) return NULL;
  t_list *tmp = head;
  for (uint64_t i = 0; i <= index; i++) {
    if (i == index) {
      return tmp;
    }
    tmp = tmp->next;
  }
  return NULL;
}

uint64_t lst_len(t_list *head) {
  uint64_t count = 0;
  for (t_list* tmp = head; tmp; tmp = tmp->next) count++;
  return count;
}

void print_32_bit_symbol_table_entry(t_symbol* symbol) {
  uint16_t symbol_header_index = read_as_uint16_t(((Elf32_Sym*)symbol->symbol_ptr)->st_shndx);
  uint32_t value = read_as_uint32_t(((Elf32_Sym*)symbol->symbol_ptr)->st_value);
  if (symbol_header_index != SHN_UNDEF)
    printf("%08x %1c %s\n", value, symbol->type, symbol->name);
  else
    printf("%s %1c %s\n", "        ", symbol->type, symbol->name);
}


void print_elf_32_symbols(t_list** head, e_cli_args* args) {

  if (!is_arg_set(P, args)) {
    DBG("%s\n", "p flag is set");
    sort_lst(*head, sort_symbol_by_value_asc);
    if (!is_arg_set(R, args)) {
    DBG("%s\n", "r flag is set");
      sort_lst(*head, sort_symbol_asc);
    }
    else sort_lst(*head, sort_symbol_dsc);
  }
  uint64_t list_len = lst_len(*head);
  DBG("lst len = %ld\n", list_len);
  for (uint64_t i = 0; i < list_len && *head; ++i) {
    t_list* node = get_symbol_at_index(*head, i, list_len);
    t_symbol* symbol = node->content;
    DBG("i == %ld | type == %c | value = %8x | name = %s\n", i, symbol->type, symbol->value, symbol->name);
    Elf32_Sym* symbol_ptr = symbol->symbol_ptr;
    uint16_t symbol_table_segment_header_entry = read_as_uint16_t(symbol_ptr->st_shndx);
    if (is_arg_set(U, args)) {
    DBG("%s\n", "u flag is not set");
      if (symbol_table_segment_header_entry == SHN_UNDEF) {
        print_32_bit_symbol_table_entry(symbol);
      }
    } else if (is_arg_set(G, args)) {
      if (ELF32_ST_BIND(symbol_ptr->st_info) == STB_GLOBAL || ELF32_ST_BIND(symbol_ptr->st_info) == STB_WEAK)
        print_32_bit_symbol_table_entry(symbol);
    } else if (is_arg_set(A, args)) {
    DBG("%s\n", "a flag is not set");
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

