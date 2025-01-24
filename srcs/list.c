#include "../includes/list.h"

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

uint64_t lst_len(t_list *head) {
  uint64_t count = 0;
  for (t_list* tmp = head; tmp; tmp = tmp->next) count++;
  return count;
}

void sort_lst(t_list* list, )

void print_elf_32_symbols(t_list* head, e_cli_args* args) {

  if (!is_arg_set(P, args)) {

  }

}

