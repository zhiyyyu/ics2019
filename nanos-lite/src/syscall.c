#include "common.h"
#include "syscall.h"
#include <am.h>
#include "debug.h"
#include "fs.h"

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  // Log("a[0] %d, a[1] %d, a[2] %d, a[3] %d", a[0], a[1], a[2], a[3]);

  switch (a[0]) {
  case SYS_exit: 
    // Log("sys_exit");
    _halt(a[1]); return NULL;
  case SYS_yield: 
    // Log("sys_yield");
    _yield(); return NULL;
  case SYS_write: 
    // Log("sys_write");
    c->GPRx = fs_write(a[1], (void*) a[2], a[3]); 
    return NULL;
  case SYS_read: 
    // Log("sys_read");
    c->GPRx = fs_read(a[1], (void*) a[2], a[3]); 
    return NULL;
  case SYS_open: 
    // Log("sys_open");
    c->GPRx = fs_open((char*) a[1], a[2], a[3]); 
    return NULL;
  case SYS_close: 
    // Log("sys_close");
    c->GPRx = fs_close(a[1]); 
    return NULL;
  case SYS_lseek: 
    // Log("sys_lseek");
    c->GPRx = fs_lseek(a[1], a[2], a[3]); 
    return NULL;
  case SYS_brk:
    // Log("sys_brk");
    return NULL;
  default: panic("Unhandled syscall ID = %d", a[0]);
  }
  return NULL;
}
