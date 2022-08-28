#include "cpu/exec.h"

make_EHelper(csrrs) {
  TODO();
}

make_EHelper(csrrw) {
  rtlreg_t* csr = getCSRs(id_src2->val);
  rtl_mv(&s0, csr);
  rtl_mv(csr, &id_src->val);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(csrrw);
}

make_EHelper(ecall) {
  TODO();
}