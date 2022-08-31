#include "common.h"
#include <amdev.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
  for(int i=0;i<len;i++){
    _putc(((char *)buf)[i]);
  }
  return len;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
  int key = read_key();
  int time = uptime();
  char* p = buf;
  if((key & 0x7fff) == _KEY_NONE){
    Log("time");
    len = sprintf(p, "t %d\n", time);
  } else if(key & 0x8000){
    Log("kd");
    len = sprintf(buf, "kd %s\n", keyname[key & 0x7fff]);
  } else {
    Log("ku");
    len = sprintf(buf, "ku %s\n", keyname[key & 0x7fff]);
  }
  Log("%s, %d", buf, len);
  return len;
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
