#include "nemu.h"
#include "monitor/diff-test.h"

extern const char *regsl[];

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  bool success = true;
  if(ref_r->pc != cpu.pc){
    Log("pc differs: ref %x, pc %x", ref_r->pc, cpu.pc);
    success = false;
  }
  for(int i=0;i<32;i++){
    if(ref_r->gpr[i]._32 != cpu.gpr[i]._32){
      Log("%s reg differs: %d %d", 
        regsl[i], ref_r->gpr[i]._32, cpu.gpr[i]._32);
      success = false;
    }
  }
  return success;
}

void isa_difftest_attach(void) {
}
