.thumb
.thumb_func
.global _start
_start:
stacktop: .word 0x20001000
.word reset
@ .word Myexit
@ .word hang

.thumb_func
reset:
    bl notmain
    bl Myexit
    b hang

.thumb_func
hang:   b .

.thumb_func
.globl PUT32
PUT32:
    str r1,[r0]
    bx lr
