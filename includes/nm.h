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
#include "list.h"

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

