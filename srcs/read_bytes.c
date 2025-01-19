#include "../includes/nm.h"

static void flip_endian(void *ptr, size_t s) {
  uint8_t *data = (uint8_t*)ptr;
  uint8_t buff[32] = {0};
  for (size_t i = 0; i < s; ++i) {
    buff[i] = data[s - 1 - i];
  }

  for (size_t i = 0; i < s; ++i) {
    data[i] = buff[i];
  }
}


uint32_t read_as_uint32_t(uint32_t value) {
  uint32_t res = value;
  if (!is_elf_byte_order_matching_os(NULL)) {
    flip_endian(&res, sizeof(uint32_t));
  }
  return res;
}

uint16_t read_as_uint16_t(uint16_t value) {
  uint16_t res = value;
  if (!is_elf_byte_order_matching_os(NULL)) {
    flip_endian(&res, sizeof(uint16_t));
  }
  return res;

}

