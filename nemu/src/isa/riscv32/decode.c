#include "cpu/decode.h"
#include "rtl/rtl.h"

// decode operand helper
#define make_DopHelper(name) void concat(decode_op_, name) (Operand *op, uint32_t val, bool load_val)

static inline make_DopHelper(i) {
  op->type = OP_TYPE_IMM;
  op->imm = val;
  rtl_li(&op->val, op->imm);

  print_Dop(op->str, OP_STR_SIZE, "%d", op->imm);
}

static inline make_DopHelper(r) {
  op->type = OP_TYPE_REG;
  op->reg = val;
  // load_val 将val同时复制到op->val
  // 注意reg会覆盖simm、addr等
  if (load_val) {
    rtl_lr(&op->val, op->reg, 4);
  }

  print_Dop(op->str, OP_STR_SIZE, "%s", reg_name(op->reg, 4));
}

make_DHelper(U) {
  decode_op_i(id_src, decinfo.isa.instr.imm31_12 << 12, true);
  decode_op_r(id_dest, decinfo.isa.instr.rd, false);

  print_Dop(id_dest->str, OP_STR_SIZE, "%s", reg_name(id_dest->reg, 4));
  print_Dop(id_src->str, OP_STR_SIZE, "0x%x", decinfo.isa.instr.imm31_12);
}

make_DHelper(ld) {
  decode_op_r(id_src, decinfo.isa.instr.rs1, true);
  decode_op_i(id_src2, decinfo.isa.instr.simm11_0, true);

  print_Dop(id_src->str, OP_STR_SIZE, "%d(%s)", id_src2->val, reg_name(id_src->reg, 4));

  rtl_add(&id_src->addr, &id_src->val, &id_src2->val);

  // dst 就不需要load_val了
  decode_op_r(id_dest, decinfo.isa.instr.rd, false);
}

make_DHelper(st) {
  decode_op_r(id_src, decinfo.isa.instr.rs1, true);
  int32_t simm = (decinfo.isa.instr.simm11_5 << 5) | decinfo.isa.instr.imm4_0;
  decode_op_i(id_src2, simm, true);

  print_Dop(id_src->str, OP_STR_SIZE, "%d(%s)", id_src2->val, reg_name(id_src->reg, 4));

  rtl_add(&id_src->addr, &id_src->val, &id_src2->val);

  decode_op_r(id_dest, decinfo.isa.instr.rs2, true);
}

make_DHelper(I) {
  decode_op_r(id_src, decinfo.isa.instr.rs1, true);
  decode_op_i(id_src2, decinfo.isa.instr.simm11_0, true);
  decode_op_r(id_dest, decinfo.isa.instr.rd, true);

  print_Dop(id_src->str, OP_STR_SIZE, "%s", reg_name(id_src->reg, 4));
  print_Dop(id_src2->str, OP_STR_SIZE, "%d", id_src2->val);
  print_Dop(id_dest->str, OP_STR_SIZE, "%s", reg_name(id_dest->reg, 4));
}

make_DHelper(J) {
  int32_t simm = (decinfo.isa.instr.imm10_1 << 1) |
                (decinfo.isa.instr.imm11_ << 11) | 
                (decinfo.isa.instr.imm19_12 << 12) |
                (decinfo.isa.instr.simm20&1 << 20);
  Log("j simm: %x %d", simm, simm);
  Log("10_1 %x, 11 %x, 19_12 %x, 20 %x", decinfo.isa.instr.imm10_1,
  decinfo.isa.instr.imm11_,
  decinfo.isa.instr.imm19_12,
  decinfo.isa.instr.simm20&1);
  decode_op_i(id_src, simm, true);
  decode_op_r(id_dest, decinfo.isa.instr.rd, false);

  print_Dop(id_dest->str, OP_STR_SIZE, "%s", reg_name(id_dest->reg, 4));
  print_Dop(id_src->str, OP_STR_SIZE, "0x%x(%d)", id_src->val, id_src->val);
}

make_DHelper(R){
  decode_op_r(id_src, decinfo.isa.instr.rs1, true);
  decode_op_r(id_src2, decinfo.isa.instr.rs2, true);
  decode_op_r(id_dest, decinfo.isa.instr.rd, false);

  print_Dop(id_src->str, OP_STR_SIZE, "%s", reg_name(id_src->reg, 4));
  print_Dop(id_src2->str, OP_STR_SIZE, "%s", reg_name(id_src2->reg, 4));
  print_Dop(id_dest->str, OP_STR_SIZE, "%s", reg_name(id_dest->reg, 4));
}

make_DHelper(B) {
  decode_op_r(id_src, decinfo.isa.instr.rs1, true);
  decode_op_r(id_src2, decinfo.isa.instr.rs2, true);
  int32_t simm = (decinfo.isa.instr.imm4_1 >> 1) |
                (decinfo.isa.instr.imm10_5 >> 5) |
                (decinfo.isa.instr.imm11 >> 11) |
                (decinfo.isa.instr.simm12 >> 12);
  decode_op_i(id_dest, simm, false);

  print_Dop(id_src->str, OP_STR_SIZE, "%s", reg_name(id_src->reg, 4));
  print_Dop(id_src2->str, OP_STR_SIZE, "%s", reg_name(id_src2->reg, 4));
  print_Dop(id_dest->str, OP_STR_SIZE, "0x%x", id_dest->val);
}