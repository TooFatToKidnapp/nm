#ifndef LIST_H
#define LIST_H

#include "nm.h"

typedef struct symbol {
  char *name;
  uint64_t value;
  uint8_t type;
  void *symbol_ptr;
  uint16_t segment_header_index;
} t_symbol;

typedef struct list {
  struct list *next, *prev;
  t_symbol *content;
} t_list;


void clear_lst(t_list **head);
t_list *build_node(t_symbol *content);
void push_back_node(t_list **head, t_symbol *content);
void print_lst(t_list **head);
t_symbol *format_symbol(char *name, uint8_t type, uint64_t value, Elf32_Sym *ptr, uint16_t index);

void print_elf_32_symbols(t_list** head, e_cli_args* args);

#endif
