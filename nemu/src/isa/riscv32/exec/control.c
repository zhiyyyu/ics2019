#include "cpu/exec.h"

make_EHelper(jal) {
	rtl_addi(&s0, &cpu.pc, 4);
	rtl_sr(id_dest->reg, &s0, 4);
	rtl_addi(&s0, &cpu.pc, id_src->val);
	rtl_j(s0);
	print_asm_template2(jal);
}

make_EHelper(jalr) {
	rtl_addi(&s0, &cpu.pc, 4);
	rtl_sr(id_dest->reg, &s0, 4);

	rtl_addi(&s0, &id_src->reg, id_src2->val);
	s0 &= ~1;
	rtl_j(s0);
	print_asm_template3(jalr);
}