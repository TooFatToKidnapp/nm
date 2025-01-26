#include "../includes/nm.h"

static void swap_symbols(t_list* lhs, t_list* rhs) {
  if (!lhs || !rhs || lhs == rhs) return;
  t_symbol* tmp = lhs->content;
  lhs->content = rhs->content;
  rhs->content = tmp;
}


void sort_lst(t_list* list, int (*cmp) (t_list* lhs, t_list* rhs)) {
  uint64_t list_len;

  if (!list || !cmp || 1 >=(list_len = lst_len(list))) return;
  uint32_t i;
  uint32_t j = list_len;
  bool swap = true;

  while (swap) {
    i = 0;
    swap = false;
    while (i + 1 < j) {
      t_list* lhs = get_symbol_at_index(list, i, list_len);
      t_list* rhs = get_symbol_at_index(list, i + 1, list_len);
      if (cmp(lhs, rhs) > 0) {
        swap = true;
        swap_symbols(lhs, rhs);
      }
      i++;
    }
    j--;
  }
}

static int32_t item_cmp(t_list* lhs, t_list* rhs) {
  const t_symbol *lhs_symbol = lhs->content;
  const t_symbol *rhs_symbol = rhs->content;
  const char *l_name = lhs_symbol->name;
  const char *r_name = rhs_symbol->name;
  uint64_t l_name_len = _strlen(l_name);
  uint64_t r_name_len = _strlen(r_name);
  int32_t cmp = case_insensitive_strncmp(r_name, l_name, (l_name_len > r_name_len ? l_name_len : r_name_len));
  if (cmp == 0) cmp = lhs_symbol->segment_header_index - rhs_symbol->segment_header_index;
  if (cmp == 0) {
    if (rhs_symbol->type == 'W' && lhs_symbol->type == 'D') cmp = -1;
    else if (rhs_symbol->type == 'D' && lhs_symbol->type == 'W') cmp = 1;
    else if (rhs_symbol->type == 't' && lhs_symbol->type == 'T') cmp = -1;
    else if (rhs_symbol->type == 'T' && lhs_symbol->type == 't') cmp = 1;
    else if (rhs_symbol->type == 'T' && lhs_symbol->type == 'W') cmp = -1;
    else if (rhs_symbol->type == 'D' && lhs_symbol->type == 'T') cmp = 1;
  }
  if (cmp == 0) {
    return l_name_len - r_name_len;
  }
  return cmp;
}

int32_t sort_symbol_by_value_asc(t_list* lhs, t_list* rhs) {
  if (!lhs || !rhs) return 0;
  t_symbol* lhs_sym = lhs->content;
  t_symbol* rhs_sym = rhs->content;
  if (!lhs_sym || !rhs_sym) return 0;
  return lhs_sym->value - rhs_sym->value;
}

int32_t sort_symbol_asc(t_list* lhs, t_list* rhs) {
  return item_cmp(lhs, rhs);
}

int32_t sort_symbol_dsc(t_list* lhs, t_list* rhs) {
  return item_cmp(rhs, lhs);
}
