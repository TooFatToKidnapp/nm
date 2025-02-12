#include "../includes/nm.h"

static int32_t get_platform_byte_order() {
  uint32_t i = 1;
  /*
    ** i == 00000001 in memory
    ** *(char*)&i pointes to the first byte of i
    ** if that byte is 1 then Little-endian byte order
  */
  if (*(char*)&i == 1)
    return (ELFDATA2LSB);
  else
    return (ELFDATA2MSB);

}

bool is_elf_byte_order_matching_os(char * file) {
  static bool _init = false;
  static unsigned char os_byte_order;
  static unsigned char elf_file_byte_order;
  if (_init == false) {
    os_byte_order = get_platform_byte_order();
    elf_file_byte_order = file[EI_DATA];
    _init = true;
  }
  if (os_byte_order == elf_file_byte_order) return true;
  else return false;
}


/// validates the elf file header
/// check's the following:
///  - the files first 4 magic number's to verify the file type
///  - the CLASS byte that identifies the suported architecture. 64 or 32 bit
///  - the DATA byte that specifies the data encoding. little-endian or big-endian
///  - the VERSION byte that specifies the version number of the ELF specification
bool is_valid_elf_file_ident(t_elf_file *file) {

  if (16 > file->file_size) false;
  is_elf_byte_order_matching_os(file->file_content);
  if (_memcmp(file->file_content, ELFMAG, SELFMAG) != 0
    || file->file_content[EI_CLASS] <= 0 || file->file_content[EI_CLASS] > 2
    || file->file_content[EI_DATA] <= 0 || file->file_content[EI_DATA] > 2
    || file->file_content[EI_VERSION] != EV_CURRENT) return false;
  else return true;
}



void nm(char * file_path, e_cli_args* args) {
  t_elf_file file_info = {0};
  file_info.file_fd = get_file_handler(file_path);
  struct stat file_stat = {0};

  if (0 > fstat(file_info.file_fd, &file_stat)) {
    clean_up(NULL, 0 , file_info.file_fd);
    panic("Failed to get file stat", 1);
  }
  file_info.file_size = file_stat.st_size;
  file_info.file_content = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, file_info.file_fd, 0);
  if (file_info.file_content == MAP_FAILED) {
    clean_up(NULL, 0 , file_info.file_fd);
    panic("Failed to load file into memory", 1);
  }
  file_info.file_name = file_path;
  if (is_valid_elf_file_ident(&file_info) == false) {
    clean_up(file_info.file_content, file_info.file_size , file_info.file_fd);
    panic("Invalid elf file Indent", 1);
  }
  if (file_info.file_content[EI_CLASS] == ELFCLASS64) {
    // 64 bit adder nm implementation
  } else {
    // DBG("%s\n", "32 bit elf format");
    nm32(&file_info, args);
  }
  (void) args;

}
