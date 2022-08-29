#include "cpu/exec.h"

make_EHelper(csrrs) {
  rtlreg_t* csr = getCSRs(id_src2->val);
  rtl_mv(&s0, csr);
  rtl_or(&s1, &s0, &id_src->val);
  rtl_mv(csr, &s1);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(csrrs);
}

make_EHelper(csrrw) {
  rtlreg_t* csr = getCSRs(id_src2->val);
  rtl_mv(&s0, csr);
  rtl_mv(csr, &id_src->val);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(csrrw);
}

make_EHelper(ecall) {
  raise_intr(reg_l(17), cpu.pc);
  print_asm("ecall");
}

make_EHelper(sret) {
  return_intr();
  print_asm("sret");
}