#include "rtl/rtl.h"

void raise_intr(uint32_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  rtl_li(&sepc, epc);
  rtl_li(&scause, NO);
  rtl_mv(&s0, &stvec);
  // Log("ecall jump to %x", s0);
  // Log("scause: %d, sstatus %d, sepc 0X%x, stvec 0X%x", scause, sstatus, sepc, stvec);
  rtl_j(s0);
}

void return_intr() {
  rtl_mv(&s0, &sepc);
  // Log("sret jump to %x", s0 + 4);
  rtl_j(s0 + 4);
}

bool isa_query_intr(void) {
  return false;
}
