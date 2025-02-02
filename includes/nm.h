#ifndef NM_H
#define NM_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ar.h>
#include <elf.h>

#define DBG(fmt, ...) \
  fprintf(stderr, "DEBUG: %s:%d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__);


typedef enum cli_args {
  P = 1 << 0,
  G = 1 << 1,
  R = 1 << 2,
  U = 1 << 3,
  A = 1 << 4,
} e_cli_args;


typedef struct elf_file {
  char* file_name;
  char* file_content;
  uint64_t     file_size;
  int32_t     file_fd;
} t_elf_file;

typedef struct symbol {
  char *name;
  uint32_t value;
  uint8_t type;
  void *symbol_ptr;
  uint16_t segment_header_index;
} t_symbol;

typedef struct list {
  struct list *next, *prev;
  t_symbol *content;
} t_list;


/*
  ** list
*/

void clear_lst(t_list **head);
t_list *build_node(t_symbol *content);
void push_back_node(t_list **head, t_symbol *content);
void print_lst(t_list **head);
t_symbol *format_symbol(char *name, uint8_t type, uint64_t value, Elf32_Sym *ptr, uint16_t index);
void print_elf_32_symbols(t_list** head, e_cli_args* args);
uint64_t lst_len(t_list *head);
t_list * get_symbol_at_index(t_list* head, uint64_t index, uint64_t list_len);



/*
  ** sort
*/

void sort_lst(t_list* list, int (*cmp) (t_list* lhs, t_list* rhs));
int32_t sort_symbol_dsc(t_list* lhs, t_list* rhs);
int32_t sort_symbol_asc(t_list* lhs, t_list* rhs);
int32_t sort_symbol_by_value_asc(t_list* lhs, t_list* rhs);


/*
  ** utils
*/

uint64_t _strlen(const char * str);
uint64_t  _strlcat(char *dst, const char *src, uint64_t dstsize);
int32_t _strncmp(const char *s1, const char *s2, uint32_t n);
int32_t _memcmp(const void *s1, const void *s2, uint64_t n);
bool is_elf_byte_order_matching_os(char * file);
int32_t case_insensitive_strncmp(const char* s1, const char* s2, uint32_t len);
uint32_t read_as_uint32_t(uint32_t ptr);
uint16_t read_as_uint16_t(uint16_t ptr);
uint32_t get_32_bit_symbol_type(Elf32_Ehdr *ehdr, Elf32_Shdr* shdr, Elf32_Sym *symbol);
uint8_t match_section_type(char* section_name, uint8_t type, uint32_t bind);

/*
  ** err handlers
*/

void panic(const char * msg, int32_t status_code);
void clean_up(void * file_mapping, size_t file_size, int32_t fd);

/*
  ** argument validation
*/

int32_t get_file_handler(char *file_path);
uint32_t parse_and_count_cli_args(char ** arg_list, e_cli_args* args);
bool is_arg_set(e_cli_args arg, e_cli_args * args);
bool is_arg(char * arg);

/*
  ** src implementation
*/

void nm(char * file_path, e_cli_args* args);
int32_t nm32(t_elf_file* file_info, e_cli_args* args);

#endif

