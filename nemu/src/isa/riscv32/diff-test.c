#include "nemu.h"
#include "monitor/diff-test.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  Log("ref: %x, pc: %x, cpu.pc: %x", ref_r->pc, pc, cpu.pc);
  if(ref_r->pc != cpu.pc) return false;
  for(int i=0;i<32;i++){  
    Log("ref: %x, pc: %x, cpu.pc: %x", ref_r->pc, pc, cpu.pc);
    if(ref_r->gpr[i]._32 != cpu.gpr[i]._32) return false;
  }
  return true;
}

void isa_difftest_attach(void) {
}
