#include "common.h"
#include "syscall.h"
#include <am.h>

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  printf("a[0] %d, a[1] %d, a[2] %d, a[3] %d\n", a[0], a[1], a[2], a[3]);

  switch (a[0]) {
  case SYS_exit: 
    printf("sys_exit\n");
    _halt(a[1]); return NULL;
  case SYS_yield: 
    printf("sys_yield\n");
    _yield(); return NULL;
  case SYS_write: 
    printf("sys_write\n");
    c->GPRx = fs_write(a[1], (void*) a[2], a[3]); return NULL;
  default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
