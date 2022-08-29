#include "rtl/rtl.h"

void raise_intr(uint32_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  rtl_li(&sepc, epc);
  rtl_li(&scause, NO);
  rtl_mv(&s0, &stvec);
  Log("ecall jump to %x", s0);
  // Log("scause: %d, sstatus %d, sepc 0X%x\n", scause, sstatus, sepc);
  rtl_j(s0);
}

void return_intr() {
  rtl_mv(&s0, &sepc);
  rtl_j(s0);
}

bool isa_query_intr(void) {
  return false;
}
