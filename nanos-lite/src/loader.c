#include "proc.h"
#include <elf.h>

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

// 用于载入用户程序
static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr ehdr;
  // 读取Elf头
  ramdisk_read(&ehdr, 0, sizeof(Elf_Ehdr));
  Elf_Phdr phdr[ehdr.e_phnum];
  ramdisk_read(phdr, ehdr.e_ehsize, sizeof(Elf_Phdr)*ehdr.e_phnum);
  for(size_t i=0;i<ehdr.e_phnum;i++){
    if(phdr[i].p_type == PT_LOAD){
      ramdisk_read((void*)phdr[i].p_vaddr, phdr[i].p_offset, phdr[i].p_memsz);
      memset((void*)(phdr[i].p_vaddr + phdr[i].p_filesz), 0
        , phdr[i].p_memsz - phdr[i].p_filesz);
    }
  }
  return ehdr.e_entry;
}

extern void difftest_dettach();
extern void difftest_attach();

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = 0X%x", entry);
#ifdef DIFF_TEST
  difftest_attach();
#endif
  ((void(*)())entry) ();
#ifdef DIFF_TEST
  difftest_dettach();
#endif
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
