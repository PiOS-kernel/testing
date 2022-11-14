.syntax unified
.thumb
.global SVCallISR
SVCallISR:
    MOV R0, #0x0
    ADD R0, R0, #0xc000
    ADD R0, R0, #0x40000000
    MOV R1, #0x64
    B PUT32
	@ TST		LR, #4
    @ @ IfElse Thumb condition based on flags set by the previous instruction
    @ @ IT pattern depends on how many conditions are indicated
    @ ITE     EQ
	@ MRSEQ	R0, MSP
	@ MRSNE	R0, PSP
	
	@ LDR		R0, [R0, #24]
	@ LDRB	R0, [R0, #-2]
	
	@ CMP		R0, #01
    @ ITT     EQ
	@ LDREQ	R1, =Service_Call_1
	@ BEQ		Done
	
	@ CMP		R0, #02
    @ ITT     EQ
	@ LDREQ	R1, =Service_Call_2
	@ BEQ		Done

	@ LDR		R1, =Service_Call_Default

Done:
	PUSH 	{LR}
	BLX		R1
	POP		{PC}
