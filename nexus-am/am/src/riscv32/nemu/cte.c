#include <am.h>
#include <riscv32.h>
#include <klib.h>

static _Context* (*user_handler)(_Event, _Context*) = NULL;

/**
 * @brief: 恢复上下文
 * @param {_Context} *c
 */
_Context* __am_irq_handle(_Context *c) {
  // printf("__am_irq_handle: ");
  // printf("scause: %d, sstatus %d, sepc 0X%x\n", c->cause, c->status, c->epc);
  
  _Context *next = c;
  if (user_handler) {
    _Event ev = {0};
    switch (c->cause) {
    case 0xffffffff: ev.event = _EVENT_YIELD; break;
    case 0: case 1: case 2: case 3: case 4: case 5: case 6:
    case 7: case 8: case 9: case 10: case 11: case 12: case 13:
    case 14: case 15: case 16: case 17: case 18: case 19:
      ev.event = _EVENT_SYSCALL; 
      break;
    default: ev.event = _EVENT_ERROR; break;
    }

    next = user_handler(ev, c);
    if (next == NULL) {
      next = c;
    }
  }

  return next;
}

extern void __am_asm_trap(void);

int _cte_init(_Context*(*handler)(_Event, _Context*)) {
  // initialize exception entry
  // %0是占位符，第一个冒号后面表示输出，第二个冒号后面表示输入
  // "r" 表示输入可以是寄存器
  // stvec中保存异常入口地址
  asm volatile("csrw stvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return 0;
}

/**
 * @brief: 内核上下文
 *    用于为新程序创建上下文，并返回cp
 * @param {_Area} stack
 * @param {void} * entry 返回地址
 * @param {void} *arg 可以先忽略
 */
_Context *_kcontext(_Area stack, void (*entry)(void *), void *arg) {
  _Context* p = (_Context*)&stack;
  // TODO: set entry as return addr
  return p;
}

void _yield() {
  asm volatile("li a7, -1; ecall");
}

int _intr_read() {
  return 0;
}

void _intr_write(int enable) {
}
