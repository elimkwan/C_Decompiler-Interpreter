         db "Hello world!\n"
main:    mov %a,0x04
         mov %b,0x01
         mov %c,0x00
         mov %d,0x0d
         syscall
         mov %a,0x01
         syscall
