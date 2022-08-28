#include "cpu/exec.h"
#include "all-instr.h"

static OpcodeEntry load_table [8] = {
  EXW(ld, 1), EXW(ld, 2), EXW(ld, 4), EMPTY, EXW(ldu, 1), EXW(ldu, 2), EMPTY, EMPTY
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

static inline uint32_t highbit(uint32_t x){
  switch (x){
  case 0: case 1: return x;
  case 32: return 6;
  default: TODO();
  }
}

static make_EHelper(add_sub_mul) {
  // Log("add_sub_mul: %x %d", decinfo.isa.instr.funct7, lowbit(decinfo.isa.instr.funct7));
  decinfo.width = as_table[highbit(decinfo.isa.instr.funct7)].width;
  idex(pc, &as_table[highbit(decinfo.isa.instr.funct7)]);
}

static OpcodeEntry dx_table [2] = {
  EX(xor), EX(idiv_q)
};

static make_EHelper(xor_div) {
  decinfo.width = dx_table[decinfo.isa.instr.funct7].width;
  idex(pc, &dx_table[decinfo.isa.instr.funct7]);
}

static OpcodeEntry or_table [2] = {
  EX(or), EX(idiv_r)
};

static make_EHelper(or_rem) {
  decinfo.width = or_table[decinfo.isa.instr.funct7].width;
  idex(pc, &or_table[decinfo.isa.instr.funct7]);
}

static OpcodeEntry sm_table [2] = {
  EX(sltu), EX(mul_hi)
};

static make_EHelper(sltu_mulhu) {
  decinfo.width = sm_table[decinfo.isa.instr.funct7].width;
  idex(pc, &sm_table[decinfo.isa.instr.funct7]);
}

static OpcodeEntry si_table [2] = {
  EX(shl), EX(imul_hi)
};

static make_EHelper(shl_mulh) {
  decinfo.width = si_table[decinfo.isa.instr.funct7].width;
  idex(pc, &si_table[decinfo.isa.instr.funct7]);
}

static OpcodeEntry ah_table [7] = {
  EX(shr), EX(div_q), EMPTY, EMPTY, EMPTY, EMPTY, EX(sar), EMPTY
};

static make_EHelper(sra_srl) {
  decinfo.width = ah_table[highbit(decinfo.isa.instr.funct7)].width;
  idex(pc, &ah_table[highbit(decinfo.isa.instr.funct7)]);
}

static OpcodeEntry and_remu_table [2] = {
  EX(and), EX(idiv_r)
};

static make_EHelper(and_remu) {
  decinfo.width = and_remu_table[decinfo.isa.instr.funct7].width;
  idex(pc, &and_remu_table[decinfo.isa.instr.funct7]);
}

static OpcodeEntry r_table [8] = {
  EX(add_sub_mul), EX(shl_mulh), EX(slt), EX(sltu_mulhu), EX(xor_div), EX(sra_srl), EX(or_rem), EX(and_remu)
};

static make_EHelper(r_type) {
  decinfo.width = r_table[decinfo.isa.instr.funct3].width;
  idex(pc, &r_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry ahi_table [2] = {
  EX(shri), EX(sari)
};

static make_EHelper(sari_shri) {
  decinfo.width = ahi_table[decinfo.isa.instr.funct7 >> 5].width;
  idex(pc, &ahi_table[decinfo.isa.instr.funct7 >> 5]);
}

static OpcodeEntry i_table [8] = {
  EX(addi), EX(shli), EMPTY, EX(sltiu), EX(xori), EX(sari_shri), EX(ori), EX(andi)
};

static make_EHelper(i_type) {
  decinfo.width = i_table[decinfo.isa.instr.funct3].width;
  idex(pc, &i_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry b_table [8] = {
  EX(beq), EX(bne), EMPTY, EMPTY, EX(blt), EX(bge), EX(bltu), EX(bgeu)
};

static make_EHelper(b_type) {
  decinfo.width = b_table[decinfo.isa.instr.funct3].width;
  idex(pc, &b_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry csr_table [8] = {
  EX(ecall), EX(csrrw), EX(csrrs), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(csr) {
  decinfo.width = csr_table[decinfo.isa.instr.funct3].width;
  idex(pc, &csr_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry opcode_table [32] = {
  /* b00 */ IDEX(ld, load), EMPTY, EMPTY, EMPTY, IDEX(I, i_type), IDEX(U, auipc), EMPTY, EMPTY,
  /* b01 */ IDEX(st, store), EMPTY, EMPTY, EMPTY, IDEX(R, r_type), IDEX(U, lui), EMPTY, EMPTY,
  /* b10 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b11 */ IDEX(B, b_type), IDEX(I, jalr), EX(nemu_trap), IDEX(J, jal), IDEX(I, csr), EMPTY, EMPTY, EMPTY,
};

void isa_exec(vaddr_t *pc) {
  // 取指
  decinfo.isa.instr.val = instr_fetch(pc, 4);
  assert(decinfo.isa.instr.opcode1_0 == 0x3);

  // 译码、执行
  idex(pc, &opcode_table[decinfo.isa.instr.opcode6_2]);
}
