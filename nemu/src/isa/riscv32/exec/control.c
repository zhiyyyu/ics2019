#include "cpu/exec.h"

make_EHelper(jal) {
	rtl_mv(&id_dest->reg, pc);
	rtl_subi(&s0, pc, 4);
	rtl_addi(pc, &s0, id_src->val);

	print_asm_template2(jal);
}

