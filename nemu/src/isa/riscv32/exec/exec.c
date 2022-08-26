#include "cpu/exec.h"
#include "all-instr.h"

static OpcodeEntry load_table [8] = {
  EMPTY, EMPTY, EXW(ld, 4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(load) {
  decinfo.width = load_table[decinfo.isa.instr.funct3].width;
  idex(pc, &load_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry store_table [8] = {
  EMPTY, EMPTY, EXW(st, 4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(store) {
  decinfo.width = store_table[decinfo.isa.instr.funct3].width;
  idex(pc, &store_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry as_table [2] = {
  EXW(add, 4), EXW(sub, 4)
};

static make_EHelper(add_sub) {
  decinfo.width = as_table[decinfo.isa.instr.funct7 >> 5].width;
  idex(pc, &as_table[decinfo.isa.instr.funct7 >> 5]);
}

static OpcodeEntry r_table [8] = {
  EXW(add_sub, 4), EMPTY, EMPTY, EXW(sltu, 4), EXW(xor, 4), EMPTY, EXW(or, 4), EMPTY
};

static make_EHelper(r_type) {
  decinfo.width = r_table[decinfo.isa.instr.funct3].width;
  idex(pc, &r_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry i_table [8] = {
  EXW(addi, 4), EMPTY, EMPTY, EXW(sltiu, 4), EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(i_type) {
  decinfo.width = i_table[decinfo.isa.instr.funct3].width;
  idex(pc, &i_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry b_table [8] = {
  EXW(beq, 4), EXW(bne, 4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
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
