#include <am.h>
#include <amdev.h>
#include <nemu.h>

size_t __am_video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      uint32_t wh = inl(SCREEN_ADDR);
      _DEV_VIDEO_INFO_t *info = (_DEV_VIDEO_INFO_t *)buf;
      info->width = wh >> 16;
      info->height = wh & 0xffff;
      return sizeof(_DEV_VIDEO_INFO_t);
    }
    // case _DEVREG_VIDEO_FBCTL: {
    //   _DEV_VIDEO_FBCTL_t *info = (_DEV_VIDEO_FBCTL_t *)buf;
    //   info->x = 
    // }
  }
  return 0;
}

size_t __am_video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _DEV_VIDEO_FBCTL_t *ctl = (_DEV_VIDEO_FBCTL_t *)buf;

      if (ctl->sync) {
        outl(SYNC_ADDR, 0);
      }
      return size;
    }
  }
  return 0;
}

void __am_vga_init() {
  int i;
  int size = screen_width() * screen_height();
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < size; i ++) fb[i] = i;
  draw_sync();
}
