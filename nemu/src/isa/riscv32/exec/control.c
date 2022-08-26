#include "cpu/exec.h"

make_EHelper(jal) {
	rtl_addi(&s0, &cpu.pc, 4);
	rtl_sr(id_dest->reg, &s0, 4);
	rtl_addi(&s0, &cpu.pc, id_src->val);
	rtl_j(s0);
  // Log("next pc: %x", s0);
	print_asm_template2(jal);
}

make_EHelper(jalr) {
	// 立即数加
	rtl_addi(&s0, &cpu.pc, 4);
	// 从src拷贝到dst，最后一个参数为字节数
	rtl_sr(id_dest->reg, &s0, 4);

	rtl_addi(&s0, &id_src->val, id_src2->val);
	// Log("s0 %x", s0);
	s0 &= ~1;
	// 底层就是cpu.pc的赋值
	rtl_j(s0);

	print_asm_template3(jalr);
}

#define make_EHelper_B(name, op) make_EHelper(name) { \
  rtl_add(&s0, &cpu.pc, &id_dest->val); \
	rtl_jrelop(op, &id_src->val, &id_src2->val, s0); \
  print_asm_template3(name); \
}

make_EHelper_B(bne, RELOP_NE);
make_EHelper_B(beq, RELOP_EQ);
make_EHelper_B(bge, RELOP_GE);
make_EHelper_B(bgeu, RELOP_GEU);
make_EHelper_B(blt, RELOP_LT);