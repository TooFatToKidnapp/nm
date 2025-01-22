#ifndef LIST_H
#define LIST_H

#include "nm.h"

typedef struct list {
  struct list *next, *prev;
  void *content;
} t_list;

void clear_lst(t_list **head);
t_list *build_node(void *content);
void push_back_node(t_list **head, void *content);
void print_lst(t_list **head);



#endif
