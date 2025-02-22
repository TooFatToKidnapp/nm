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
#include <linux/limits.h>
#include <ctype.h>
#include <stdio.h>

#define DBG(fmt, ...) \
  fprintf(stdout, "DEBUG: %s:%d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__);


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
  uint64_t value;
  uint8_t type;
  void *symbol_ptr;
  uint16_t segment_header_index;
} t_symbol;

typedef struct list {
  struct list *next, *prev;
  t_symbol *content;
} t_list;



/*
  ** formating
*/

void print_symbol_details(uint64_t value, char type, char *name, bool is_32_sym_type, bool has_value);
void print_elf_32_symbols(t_list** head, e_cli_args* args);
void print_elf_64_symbols(t_list** head, e_cli_args* args);
t_symbol *format_symbol(char *name, uint8_t type, uint64_t value, void *ptr, uint16_t index);

/*
  ** list
*/

void clear_lst(t_list **head);
t_list *build_node(t_symbol *content);
void push_back_node(t_list **head, t_symbol *content);
uint64_t lst_len(t_list *head);
t_list * get_symbol_at_index(t_list* head, uint64_t index, uint64_t list_len);



/*
  ** sort
*/

void sort_lst(t_list* list, int64_t (*cmp) (t_list* lhs, t_list* rhs));
int64_t sort_symbol_dsc(t_list* lhs, t_list* rhs);
int64_t sort_symbol_asc(t_list* lhs, t_list* rhs);
int64_t sort_symbol_by_value_asc(t_list* lhs, t_list* rhs);


/*
  ** utils
*/

uint64_t _strlen(const char * str);
uint64_t  _strlcat(char *dst, const char *src, uint64_t dstsize);
int32_t _strncmp(const char *s1, const char *s2, uint32_t n);
int32_t _memcmp(const void *s1, const void *s2, uint64_t n);
bool is_elf_byte_order_matching_os(char * file);
uint32_t read_as_uint32_t(uint32_t ptr);
uint16_t read_as_uint16_t(uint16_t ptr);
uint64_t read_as_uint64_t(uint64_t value);
uint32_t get_32_bit_symbol_type(Elf32_Ehdr *ehdr, Elf32_Shdr* shdr, const Elf32_Sym *symbol);
uint32_t get_64_bit_symbol_type(Elf64_Ehdr *ehdr, Elf64_Shdr* shdr, const Elf64_Sym *symbol);
uint8_t match_section_type(const char* section_name, int8_t type, uint64_t bind);
int32_t	_tolower(int32_t c);
int32_t _isalnum(int32_t c);

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

int32_t nm(char * file_path, e_cli_args* args);
int32_t nm32(t_elf_file* file_info, e_cli_args* args);
int32_t nm64(t_elf_file *file_info, e_cli_args *args);

#endif

