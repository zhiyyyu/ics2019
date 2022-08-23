#ifndef __NEMU_H__
#define __NEMU_H__

#include "common.h"
#include "memory/memory.h"

#define RISCV32
#ifdef RISCV32
#include "../src/isa/riscv32/include/isa/reg.h"
#else
#include "isa/reg.h"
#endif

extern CPU_state cpu;

void isa_reg_display();

#endif