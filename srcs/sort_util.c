#include "../includes/nm.h"

void sort_lst(t_list* list, int64_t (*cmp) (t_list* lhs, t_list* rhs)) {
	t_list	*tmp;
	t_list	*tmp2;
	void	*content;
	tmp = list;
	while (tmp)
	{
		tmp2 = tmp->next;
		while (tmp2)
		{
			if (cmp(tmp, tmp2) > 0)
			{
				content = tmp->content;
				tmp->content = tmp2->content;
				tmp2->content = content;
			}
			tmp2 = tmp2->next;
		}
		tmp = tmp->next;
	}
}

int64_t compare_symbol(t_list *a, t_list *b) {
  t_symbol *symbol_a = a->content;
  t_symbol *symbol_b = b->content;
  const char *s1 = symbol_a->name;
  const char *s2 = symbol_b->name;

  if (s1 == NULL || s2 == NULL)
    return (0);
  while (*s1 && *s2) {
    while (*s1 && !_isalnum(*s1))
      s1++;
    while (*s2 && !_isalnum(*s2))
      s2++;
    if (_tolower(*s1) != _tolower(*s2)) {
      return ((uint8_t) _tolower(*s1) - (uint8_t) _tolower(*s2));
    }
    s1++;
    s2++;
  }
  if (*s1 == *s2) {
    if      (symbol_b->type == 'W' && symbol_a->type == 'D') return -1;
    else if (symbol_b->type == 'D' && symbol_a->type == 'W') return 1;
    else if (symbol_b->type == 't' && symbol_a->type == 'T') return -1;
    else if (symbol_b->type == 'T' && symbol_a->type == 't') return 1;
    else if (symbol_b->type == 'T' && symbol_a->type == 'W') return -1;
    else if (symbol_b->type == 'W' && symbol_a->type == 'T') return 1;
    return (sort_symbol_by_value_asc(a, b));
  }
  return ((uint8_t) *s1 - (uint8_t) *s2);
}



int64_t sort_symbol_by_value_asc(t_list* lhs, t_list* rhs) {
  if (!lhs || !rhs) {
      return 0;
  }
  t_symbol* lhs_sym = lhs->content;
  t_symbol* rhs_sym = rhs->content;
  return lhs_sym->value - rhs_sym->value;
}

int64_t sort_symbol_asc(t_list* lhs, t_list* rhs) {
  return compare_symbol(lhs, rhs);
}

int64_t sort_symbol_dsc(t_list* lhs, t_list* rhs) {
  return compare_symbol(rhs, lhs);
}
