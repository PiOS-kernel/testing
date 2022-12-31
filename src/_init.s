	.thumb
	.global PUT32

PUT32: .asmfunc
    str r1,[r0]
    bx lr

    .endasmfunc
