#ifndef NM_H
#define NM_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>


/*
  ** utils
*/

uint64_t _strlen(const char * str);
uint64_t  _strlcat(char *dst, const char *src, uint64_t dstsize);

/*
  ** err handlers
*/

void panic(const char * msg, uint32_t status_code);

/*
  ** argument validation
*/

uint32_t get_file_handler(uint8_t *file_path);

#endif

