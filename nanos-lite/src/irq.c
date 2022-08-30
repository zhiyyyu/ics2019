#include "common.h"
#include <am.h>
#include "debug.h"

extern _Context* do_syscall(_Context *c);

static _Context* do_event(_Event e, _Context* c) {
  switch (e.event) {
  case _EVENT_YIELD: 
    // Log("get yield."); 
    break;
  case _EVENT_SYSCALL: 
    // Log("get syscall.");
    do_syscall(c); // 返回值保存在a0
    break;
  default: panic("Unhandled event ID = %d, NO = %d", e.event, e.cause);
  }
  return NULL;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}
