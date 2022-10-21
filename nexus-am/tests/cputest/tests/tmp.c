

int main() {
    asm ("csrw satp, a0"); // satp: 0x180
    asm ("csrr a0, satp"); // satp: 0x180
}