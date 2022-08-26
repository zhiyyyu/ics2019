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

make_EHelper(add);
make_EHelper(sub);
make_EHelper(xor);
make_EHelper(or);
make_EHelper(sltu);
make_EHelper(shl);
make_EHelper(and);
make_EHelper(not);

make_EHelper(jal);
make_EHelper(jalr);
make_EHelper(bne);
make_EHelper(beq);

make_EHelper(inv);
make_EHelper(nemu_trap);
