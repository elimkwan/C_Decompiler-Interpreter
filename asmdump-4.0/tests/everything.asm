         db "   "
main:    mov %a,0x03
         mov %b,0x00
         mov %c,0x00
         mov %d,0x02
         syscall
         mov %b,(0x00)
         sub %b,0x30
         mul %b,0x0a
         mov %c,(0x01)
         sub %c,0x30
         add %b,%c
         mul %b,0x02
         mov %a,0x00
         add %a,0x03
         sub %a,0x01
         mov %c,%b
         mod %c,0x0a
         add %c,0x30
         mov (%a),%c
         div %b,0x0a
         cmp %a,0x00
         jne 0x2a
         mov %a,0x04
         mov %b,0x01
         mov %c,0x00
         mov %d,0x03
         syscall
         mov %a,0x01
         syscall
