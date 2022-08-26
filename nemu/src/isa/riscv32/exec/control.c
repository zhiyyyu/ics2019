#include "cpu/exec.h"

make_EHelper(jal) {
	rtl_addi(&s0, &cpu.pc, 4);
	rtl_sr(id_dest->reg, &s0, 4);
	rtl_addi(&s0, &cpu.pc, id_src->val);
	rtl_j(s0);
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