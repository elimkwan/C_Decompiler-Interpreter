# C_Decompiler-Interpreter

Author: Elim Kwan

Date written: 09/03/2017

Description: The program takes a compiled file that is supposed to run in the ZX256 and decompile it into a ZX256 assembly representation. It decompile machine code back into assembly. 

Instructions for compiling the program: type ‘make’ in the directory

Expected Output:
$./asmdump 
Usage:
./asmdump.exe —-version
./asmdump.exe —-opcode OPCODE
./asmdump.exe —-hexdump [FILE]
./asmdump.exe FILE

$./asmdump —-version
3.0

$./asmdump —-opcode 0X01
mov

$./asmdump.exe —-opcode 0x02
No such instruction “0x02”

$./asmdump —-opcode 0xasd
Invalid opcode string “0xasd”

$./asmdump.exe —-hexdump Makefile
00000000  61  6c  6c  3a  20  61  73  6d  |all: asm|
00000008  64  75  6d  70  2e  65  78  65  |dump.exe|
00000010  0a  0a  61  73  6d  64  75  6d  |..asmdum|
00000018  70  2e  65  78  65  3a  20  61  |p.exe: a|
00000020  73  6d  64  75  6d  70  6d  61  |smdumpma|
00000028  69  6e  2e  63  0a  09  67  63  |in.c..gc|
00000030  63  20  2d  73  74  64  3d  63  |c -std=c|
00000038  39  39  20  2d  57  61  6c  6c  |99 -Wall|
00000040  20  61  73  6d  64  75  6d  70  | asmdump|
00000048  6d  61  69  6e  2e  63  20  2d  |main.c -|
00000050  6f  20  61  73  6d  64  75  6d  |o asmdum|
00000058  70  2e  65  78  65  0a  0a  74  |p.exe..t|
00000060  65  73  74  3a  20  61  73  6d  |est: asm|
00000068  64  75  6d  70  2e  65  78  65  |dump.exe|
00000070  0a  09  70  79  74  68  6f  6e  |..python|
00000078  20  74  65  73  74  73  2d  35  | tests-5|
00000080  2f  72  75  6e  74  65  73  74  |/runtest|
00000088  73  2d  33  2e  30  2e  70  79  |s-3.0.py|
00000090  20  2e  2f  61  73  6d  64  75  | ./asmdu|
00000098  6d  70  2e  65  78  65          |...P..|

$./asmdump.exe —-hexdump invalid_file.txt
No such file “invalid_file.txt”

$./asmdump.exe —-hexdump < tests/hello.c
00000000  2f 2a 20 68 65 6c 6c 6f  |/* hello|
00000008  2e 63 20 2a 2f 0a 0a 23  |.c */..#|
00000010  69 6e 63 6c 75 64 65 20  |include |
00000018  3c 73 74 64 69 6f 2e 68  |<stdio.h|
00000020  3e 0a 0a 69 6e 74 20 6d  |>..int m|
00000028  61 69 6e 28 69 6e 74 20  |ain(int |
00000030  61 72 67 63 2c 20 63 68  |argc, ch|
00000038  61 72 20 2a 61 72 67 76  |ar *argv|
00000040  5b 5d 29 0a 7b 0a 20 20  |[]).{.  |
00000048  70 75 74 73 28 22 48 65  |puts("He|
00000050  6c 6c 6f 20 77 6f 72 6c  |llo worl|
00000058  64 21 5c 6e 22 29 3b 0a  |d!\n");.|
00000060  20 20 72 65 74 75 72 6e  |  return|
00000068  20 30 3b 0a 7d 0a        | 0;.}.|
0000006e

$ls | ./asmdump.exe —-hexdump
00000000  43 6f 6e 76 65 72 74 4f  |ConvertO|
00000008  70 63 6f 64 65 48 65 78  |pcodeHex|
00000010  66 69 6c 65 2e 63 0a 43  |file.c.C|
00000018  6f 6e 76 65 72 74 4f 70  |onvertOp|
00000020  63 6f 64 65 48 65 78 66  |codeHexf|
00000028  69 6c 65 2e 68 0a 4d 61  |ile.h.Ma|
00000030  6b 65 66 69 6c 65 0a 52  |kefile.R|
00000038  45 41 44 4d 45 2e 74 78  |EADME.tx|
00000040  74 0a 61 73 6d 64 75 6d  |t.asmdum|
00000048  70 2e 65 78 65 0a 61 73  |p.exe.as|
00000050  6d 64 75 6d 70 6d 61 69  |mdumpmai|
00000058  6e 2e 63 0a 61 73 6d 72  |n.c.asmr|
00000060  75 6e 2e 65 78 65 0a 61  |un.exe.a|
00000068  73 6d 72 75 6e 6d 61 69  |smrunmai|
00000070  6e 2e 63 0a 64 65 63 6f  |n.c.deco|
00000078  6d 70 69 6c 65 72 2e 63  |mpiler.c|
00000080  0a 64 65 63 6f 6d 70 69  |.decompi|
00000088  6c 65 72 2e 68 0a 74 65  |ler.h.te|
00000090  73 74 73 0a              |sts.|
00000094

$./asmdump.exe tests/hello.out
         db "Hello world!\n"
main:    mov %a,0x04
         mov %b,0x01
         mov %c,0x00
         mov %d,0x0d
         syscall
         mov %a,0x01
         syscall

$./asmdump.exe tests/hello_bad.out
         db "Hello world!"
Invalid opcode 0xc0 at pos 0x0e
