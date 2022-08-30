#include "common.h"
#include "syscall.h"
#include <am.h>

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  printf("a[1] %d, a[2] %d, a[3] %d\n", a[1], a[2], a[3]);

  switch (a[0]) {
  case SYS_yield: _yield(); return NULL;
  case SYS_exit: _halt(a[1]); return NULL;
  case SYS_write: c->GPRx = fs_write(a[0], (void*) a[1], a[2]); return NULL;
  default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
