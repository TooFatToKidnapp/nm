#include "../includes/list.h"

void clear_lst(t_list **head) {
  if (head != NULL) {
    for (t_list *ptr = *head; ptr != NULL;) {
      t_list* tmp = ptr->next;
      free(ptr);
      ptr = tmp->next;
    }
  }
}

t_list *build_node(void *content) {
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


void push_back_node(t_list **head, void *content) {
  if (head != NULL) {
    t_list* ptr = *head;
    for (; ptr != NULL && ptr->next != NULL; ptr = ptr->next);
    t_list *new = build_node(content);
    ptr->next = new;
    new->prev = ptr;
  }
}

void print_lst(t_list **head) {
    if (head != NULL) {
    t_list* ptr = *head;
    for (uint32_t i = 0; ptr != NULL && ptr->next != NULL; ptr = ptr->next) {
      printf("node pos = %d | node adder = %p |content adder = %p\n", i, ptr, ptr->content);
    }
    return;
  }
  printf("========LST IS EMPTY=========\n");
}

