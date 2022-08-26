#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);

  print_asm_template2(lui);
}

#define make_EHelper_I(name) make_EHelper(name) { \
  concat(rtl_, name) (&id_dest->val, &id_src->val, id_src2->val); \
  rtl_sr(id_dest->reg, &id_dest->val, 4); \
  print_asm_template3(name); \
}

#define make_EHelper_R(name) make_EHelper(name) { \
  concat(interpret_rtl_, name) (&id_dest->val, &id_src->val, &id_src2->val); \
  rtl_sr(id_dest->reg, &id_dest->val, 4); \
  print_asm_template3(name); \
}

#define make_EHelper_RE(name, op) make_EHelper(name) { \
  rtl_setrelop(op, &id_dest->val, &id_src->val, &id_src2->val); \
  rtl_sr(id_dest->reg, &id_dest->val, 4); \
  print_asm_template3(name); \
}

make_EHelper_I(addi);

make_EHelper_R(add);
make_EHelper_R(sub);
make_EHelper_R(xor);
make_EHelper_R(or);

make_EHelper_RE(sltiu, RELOP_LTU);
make_EHelper_RE(sltu, RELOP_LT);