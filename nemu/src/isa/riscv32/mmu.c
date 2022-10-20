#include "nemu.h"
#include "memory/memory.h"
#include "cpu/exec.h"

paddr_t page_translate(vaddr_t addr) {
  uint32_t offset = addr & 0xfff;
  uint32_t vpn_0 = (addr >> 12) & 0x3ff;
  uint32_t vpn_1 = (addr >> 22) & 0x3ff;
  uint32_t satp = *getCSRs(0x180);
  satp &= 0x3fffff;
  Log("first pte: %d", (satp << 12) | (vpn_1 << 2));
  uint32_t ppn = paddr_read((satp << 12) | (vpn_1 << 2) , 4);
  assert(ppn & 1 == 1);
  ppn &= 0x3fffff << 12;
  Log("second pte: %d", (ppn << 12) | (vpn_0 << 2));
  paddr_t paddr = paddr_read((ppn << 12) | (vpn_0 << 2), 4);
  assert(paddr & 1 == 1);
  Log("page translate: 0x%x -> 0x%x", addr, paddr);
  return paddr;
}

uint32_t isa_vaddr_read(vaddr_t addr, int len) {
  if ((addr & PAGE_MASK) + len > PAGE_SIZE) {
    /* this is a special case, you can handle it later. */
    assert(0);
  }
  else {
    paddr_t paddr = page_translate(addr);
    return paddr_read(paddr, len);
  }
}

void isa_vaddr_write(vaddr_t addr, uint32_t data, int len) {
  if ((addr & PAGE_MASK) + len > PAGE_SIZE) {
    /* this is a special case, you can handle it later. */
    assert(0);
  }
  else {
    paddr_t paddr = page_translate(addr);
    paddr_write(addr, data, len);
  }
}
