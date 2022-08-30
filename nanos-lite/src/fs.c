#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, invalid_read, invalid_write},
  {"stderr", 0, 0, invalid_read, invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb

}

size_t fs_write(int fd, const void *buf, size_t len){
  size_t length = 0;
  if(fd==1 || fd==2){ // 1: stdout, 2: stderr
    for(int i=0;i<len;i++){
      _putc(((char *)buf)[i]);
    }
    length = len;
  }
  // if(file_table[fd].disk_offset+len>=file_table[fd].size)
  //   len=file_table[fd].size-file_table[fd].disk_offset;
  // if(fd>2){
  //   if(file_table[fd].write==NULL)
  //     length=ramdisk_write(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
  //   else
  //     length=file_table[fd].write(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
  //   file_table[fd].disk_offset+=length;
  // }
  return length;
}