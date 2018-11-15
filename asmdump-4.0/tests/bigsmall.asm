         db "too small\nbig enough\n"
main:    mov %a,0x00
         mov %b,0x01
         add %a,%b
         add %b,0x01
         cmp %b,0x05
         jl  0x1b
         cmp %a,0x0b
         jle 0x32
         mov %c,0x0a
         mov %d,0x0b
         jmp 0x38
         mov %c,0x00
         mov %d,0x0a
         mov %a,0x04
         mov %b,0x01
         syscall
         mov %a,0x01
         syscall
