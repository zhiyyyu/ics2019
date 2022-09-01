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
  if((key & 0x7fff) == _KEY_NONE){
    // Log("time");
    sprintf(buf, "t %d\n", time);
  } else if(key & 0x8000){
    // Log("kd");
    sprintf(buf, "kd %s\n", keyname[key & 0x7fff]);
  } else {
    // Log("ku");
    sprintf(buf, "ku %s\n", keyname[key & 0x7fff]);
  }
  // Log("%s, %d", buf, strlen(buf));
  return strlen(buf);
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  Log("dispinfo_read");
  // len = strlen(dispinfo) - offset;
  len = sprintf(buf, dispinfo+offset);
  // strncpy((char*) buf, dispinfo + offset, len);
  // Log("%d %s %s %d", strlen(buf), buf, dispinfo, offset);
  return strlen(buf);
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  // Log("fb_write");
  int w = screen_width();
  int h = screen_height();
  int x = (offset / 4) % w, y = (offset / 4) / w;
  draw_rect((uint32_t*) buf, x, y, len / 4, 1);
  return len;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  // Log("fbsync_write");
  draw_sync();
  return len;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", screen_width(), screen_height());
  
}
