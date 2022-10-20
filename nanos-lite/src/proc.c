#include "proc.h"

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
// 指向当前运行的PCB
PCB *current = NULL;

void switch_boot_pcb() {
  current = &pcb_boot;
}

/**
 * @brief: 测试函数（多道程序）
 * @param {void} *arg
 */
void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite for the %dth time!", j);
    j ++;
    _yield();
  }
}

void init_proc() {
  context_kload(&pcb[0], (void*) hello_fun);
  context_uload(&pcb[1], "/bin/init");
  // context_uload(&pcb[1], "/bin/hello");

  switch_boot_pcb();

  Log("Initializing processes...");

  // load program here
  
  // naive_uload(NULL, "/bin/init");
}

/**
 * @brief: 进程调度，用于选择待执行的进程
 * @param {_Context} *prev
 */
_Context* schedule(_Context *prev) {
  // save the context pointer
  current->cp = prev;

  // always select pcb[0] as the new process
  // current = &pcb[0];
  current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);

  // then return the new context
  return current->cp;
}
