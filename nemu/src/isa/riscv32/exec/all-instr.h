#include "cpu/exec.h"

make_EHelper(lui);
make_EHelper(auipc);
make_EHelper(ld);
make_EHelper(ldu);
make_EHelper(st);

make_EHelper(addi);
make_EHelper(sltiu);
make_EHelper(sari);
make_EHelper(andi);
make_EHelper(xori);
make_EHelper(shli);
make_EHelper(shri);
make_EHelper(ori);

make_EHelper(add);
make_EHelper(sub);
make_EHelper(xor);
make_EHelper(or);
make_EHelper(sltu);
make_EHelper(slt);
make_EHelper(shl);
make_EHelper(sar);
make_EHelper(shr);
make_EHelper(and);
make_EHelper(not);
make_EHelper(idiv_q);  // div
make_EHelper(idiv_r);  // rem
make_EHelper(div_q);  // divu
make_EHelper(div_r);  // remu
make_EHelper(mul_lo); // mul
make_EHelper(mul_hi); // mulhu
make_EHelper(imul_lo); // mul
make_EHelper(imul_hi); // mulh

make_EHelper(jal);
make_EHelper(jalr);
make_EHelper(bne);
make_EHelper(beq);
make_EHelper(bge);
make_EHelper(bgeu);
make_EHelper(blt);;
make_EHelper(bltu);

make_EHelper(inv);
make_EHelper(nemu_trap);
