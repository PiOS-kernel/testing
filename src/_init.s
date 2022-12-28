.thumb_func
.global _start
_start:
stacktop: .word __end__
.word Reset_Handler

.thumb_func
.global PUT32
PUT32:
    str r1,[r0]
    bx lr
