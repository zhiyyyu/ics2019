#include "common.h"
#include "syscall.h"
#include <am.h>

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;

  switch (a[0]) {
  case SYS_yield: _yield(); return NULL;
  case SYS_exit: _halt(a[1]); return NULL;
  default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
