#ifndef __NEMU_H__
#define __NEMU_H__

#include "common.h"
#include "memory/memory.h"
#include "isa/reg.h"

extern CPU_state cpu;

void isa_reg_display();
uint32_t isa_reg_str2val(const char *s, bool *success);
uint32_t expr(char *e, bool *success);

#endif