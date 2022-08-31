#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  size_t open_offset;
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

extern size_t serial_write(const void *buf, size_t offset, size_t len);
extern size_t events_read(void *buf, size_t offset, size_t len);

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, 0, invalid_read, serial_write},
  {"/dev/events", 0, 0, 0, events_read, invalid_write},
  {"/dev/fb", 0, 0, 0, invalid_read, invalid_write},
  {"/dev/fbsync", 0, 0, 0, invalid_read, invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))
static int SEPCIAL_DEV = 2;

#define CHECK_OFFSET(fd, offset) \ 
  assert(0 <= (offset) && (offset) <= file_table[fd].size);

extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);
extern size_t ramdisk_read(void *buf, size_t offset, size_t len);

void init_fs() {
  // TODO: initialize the size of /dev/fb
  for(int i=3;i<NR_FILES;i++){
    if(!strcmp(file_table[i].name, "/dev")){
      SEPCIAL_DEV++;
    }
  }
}

int fs_open(const char *pathname, int flags, int mode){
  // 允许对所有已存在文件读写，忽略flags和mode
  int idx = -1;
  for(int i=3;i<NR_FILES;i++){
    if(!strcmp(file_table[i].name, pathname)){
      idx = i; break;
    }
  }
  if(idx == -1){
    Log("file %s not found", pathname);
    assert(0);
  }
  return idx;
}

int fs_close(int fd) {
  assert(fd >= 0);
  file_table[fd].open_offset = 0;
  return 0;
}

size_t fs_write(int fd, const void *buf, size_t len){
  size_t length = 0;
  assert(fd >= 0);
  // if(fd == 0){  // 0: stdin
  //   return 0;
  // } else if(fd == 1 || fd == 2){ // 1: stdout, 2: stderr
  //   for(int i=0;i<len;i++){
  //     _putc(((char *)buf)[i]);
  //   }
  //   length = len;
  // } else {
    size_t offset = file_table[fd].open_offset;
    if(fd > SEPCIAL_DEV - 1) CHECK_OFFSET(fd, offset + len);
    if(file_table[fd].write){
      length = file_table[fd].write(buf, offset + file_table[fd].disk_offset, len);
    } else{
      length = ramdisk_write(buf, offset + file_table[fd].disk_offset, len);
    }
    file_table[fd].open_offset += len;
  // }
  return length;
}

size_t fs_read(int fd, void *buf, size_t len) {
  assert(fd >= 0);
  // if(fd <= 2) return 0;
  // 读取[open_offset, open_offset + len)
  size_t offset = file_table[fd].open_offset;
  // Log("fs_read: %x %d", offset, len);
  // CHECK_OFFSET(fd, offset + len);
  int length = 0;
  len = offset + len >= file_table[fd].size ? file_table[fd].size - offset : len;
  if(file_table[fd].read){
    length = file_table[fd].read(buf, offset + file_table[fd].disk_offset, len);
  } else {
    length = ramdisk_read(buf, offset + file_table[fd].disk_offset, len);
  }
  file_table[fd].open_offset += length;
  return length;
}

size_t fs_lseek(int fd, size_t offset, int whence){
  switch (whence)
  {
  case SEEK_SET:
    if(fd > SEPCIAL_DEV - 1) CHECK_OFFSET(fd, offset);
    file_table[fd].open_offset = offset;
    break;
  case SEEK_CUR:
    if(fd > SEPCIAL_DEV - 1) CHECK_OFFSET(fd, offset + file_table[fd].open_offset);
    file_table[fd].open_offset += offset;
    break;
  case SEEK_END:
    // Log("offset %d, size %d", offset, file_table[fd].size);
    if(fd > SEPCIAL_DEV - 1) CHECK_OFFSET(fd, offset + file_table[fd].size);
    file_table[fd].open_offset = file_table[fd].size + offset;
    break;
  default:
    Log("lseek %d not support.", whence);
    assert(0);
  }
  return file_table[fd].open_offset;
}