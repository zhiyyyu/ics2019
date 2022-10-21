#include "nemu.h"
#include "memory/memory.h"
#include "cpu/exec.h"

paddr_t page_translate(vaddr_t addr) {
  uint32_t offset = addr & 0xfff;
  uint32_t vpn_0 = (addr >> 12) & 0x3ff;
  uint32_t vpn_1 = (addr >> 22) & 0x3ff;
  rtlreg_t _satp = *getCSRs(0x180) >> 12;
  _satp = 0x822e5;
  // asm volatile("csrrs %0, satp" : "=r"(_satp) : );
  Log("satp: 0x%x", _satp);
  // _satp &= 0x3fffff;
  paddr_t tmp = (_satp << 12) | (vpn_1 << 2);
  Log("first pte: 0x%x", tmp);
  paddr_t ppn = paddr_read(tmp , 4);
  assert((ppn & 1) == 1);
  ppn = (ppn >> 10) & 0x3fffff;
  Log("first ppn: 0x%x", ppn);
  tmp = (ppn << 12) | (vpn_0 << 2);
  Log("second pte: 0x%x", tmp);
  paddr_t paddr = paddr_read(tmp, 4);
  assert((paddr & 1) == 1);
  Log("page translate: 0x%x -> 0x%x", addr, paddr);
  return (paddr >> 12 << 12) | offset;
  // return addr;
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
    paddr_write(paddr, data, len);
  }
}
