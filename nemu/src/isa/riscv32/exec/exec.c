#include "cpu/exec.h"
#include "all-instr.h"

static OpcodeEntry load_table [8] = {
  EMPTY, EMPTY, EXW(ld, 4), EMPTY, EXW(ldu, 1), EMPTY, EMPTY, EMPTY
};

static make_EHelper(load) {
  decinfo.width = load_table[decinfo.isa.instr.funct3].width;
  idex(pc, &load_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry store_table [8] = {
  EXW(st, 1), EXW(st, 2), EXW(st, 4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(store) {
  decinfo.width = store_table[decinfo.isa.instr.funct3].width;
  idex(pc, &store_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry as_table [7] = {
  EX(add), EX(mul_lo), EMPTY, EMPTY, EMPTY, EMPTY, EX(sub), EMPTY
};

static inline uint32_t lowbit(uint32_t x){
  switch (x){
  case 0: case 1: return x;
  case 32: return 6;
  default: TODO();
  }
}

static make_EHelper(add_sub_mul) {
  // Log("add_sub_mul: %x %d", decinfo.isa.instr.funct7, lowbit(decinfo.isa.instr.funct7));
  decinfo.width = as_table[lowbit(decinfo.isa.instr.funct7)].width;
  idex(pc, &as_table[lowbit(decinfo.isa.instr.funct7)]);
}

static OpcodeEntry dx_table [2] = {
  EX(xor), EX(div_q)
};

static make_EHelper(xor_div) {
  decinfo.width = dx_table[decinfo.isa.instr.funct7].width;
  idex(pc, &dx_table[decinfo.isa.instr.funct7]);
}

static OpcodeEntry r_table [8] = {
  EX(add_sub_mul), EX(shl), EMPTY, EX(sltu), EX(xor_div), EMPTY, EX(or), EX(and)
};

static make_EHelper(r_type) {
  decinfo.width = r_table[decinfo.isa.instr.funct3].width;
  idex(pc, &r_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry i_table [8] = {
  EX(addi), EX(shl), EMPTY, EX(sltiu), EX(xori), EX(sari), EMPTY, EX(andi)
};

static make_EHelper(i_type) {
  decinfo.width = i_table[decinfo.isa.instr.funct3].width;
  idex(pc, &i_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry b_table [8] = {
  EX(beq), EX(bne), EMPTY, EMPTY, EMPTY, EX(bge), EMPTY, EX(bgeu)
};

static make_EHelper(b_type) {
  decinfo.width = b_table[decinfo.isa.instr.funct3].width;
  idex(pc, &b_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry opcode_table [32] = {
  /* b00 */ IDEX(ld, load), EMPTY, EMPTY, EMPTY, IDEX(I, i_type), IDEX(U, auipc), EMPTY, EMPTY,
  /* b01 */ IDEX(st, store), EMPTY, EMPTY, EMPTY, IDEX(R, r_type), IDEX(U, lui), EMPTY, EMPTY,
  /* b10 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b11 */ IDEX(B, b_type), IDEX(I, jalr), EX(nemu_trap), IDEX(J, jal), EMPTY, EMPTY, EMPTY, EMPTY,
};

void isa_exec(vaddr_t *pc) {
  // 取指
  decinfo.isa.instr.val = instr_fetch(pc, 4);
  assert(decinfo.isa.instr.opcode1_0 == 0x3);

  // 译码、执行
  idex(pc, &opcode_table[decinfo.isa.instr.opcode6_2]);
}
