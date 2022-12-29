    .cpu cortex-m4
    .thumb
    .syntax unified

@ -----------------------------------------------------------

.thumb_func
.global summon_scheduler
summon_scheduler:
    @ interrupts are disabled
    cpsid i

    @ The scheduler is activated
    ldr r0, =scheduler_state
    mov r1, #1
    str r1, [r0]

    @ The state (link register, stack_pointer, XPSR) of the test runner is saved.
    @ Link register:
    ldr r0, =runner_state
    str lr, [r0]
    @ Stack pointer:
    add r0, r0, #4
    str sp, [r0]
    @ XPSR:
    add r0, r0, #4
    mrs r1, xpsr
    str r1, [r0]

    @ interrupts are enabled
    cpsie i
    
    @ The PendSV handler is triggered
    ldr r0, =IRQ_CTRL_REGISTER
    ldr r1, =PEND_SV_BIT
    ldr r0, [r0]
    ldr r1, [r1]
    str r1, [r0]


@ -----------------------------------------------------------

.thumb_func
.global PendSVISR
PendSVISR:
    @ Check wether the scheduler should be running, and wether it was running.
    ldr r0, =scheduler_state
    ldr r0, [r0]
    ldr r1, =scheduler_running
    ldr r1, [r1]

    @ If the scheduler was running, and should now stop, then restore the
    @ context of the test runner.
    cmp r0, #0
    it eq
    cmpeq r1, #1
    it eq
    beq restore_runner_context

    @ If the scheduler was not running, and should now start, call task_switch
    push {lr}
    bl task_switch
    pop {pc}

restore_runner_context:

    @ The state of the test runner is restored.
    ldr r0, =runner_state
    ldr r1, [r0] @ Link register
    add r0, r0, #4
    ldr sp, [r0] @ Stack pointer
    add r0, r0, #4
    ldr r2, [r0] @ XPSR

    @ The stack image is initialized to be compatible with the exception
    @ return protocol of the processor.

    @ XPSR
    push {r2}
    @ PC
    push {r1}
    @ LR, r12, r3, r2, r1, r0
    mov r2, #0
    push {r2}
    push {r2}
    push {r2}
    push {r2}
    push {r2}
    push {r2}
     
    @ Return to the test runner
    bx lr


@ -----------------------------------------------------------

@ The SysTick ISR
.thumb_func
.global SysTickISR
SysTickISR:
	@ The ticks counter is incremented
    ldr r0, =TICKS_COUNTER
    ldr r1, [r0]
    add r1, r1, #1
    str r1, [r0]

    @ If the ticks counter has reached the value of 10 (10ms is the time
    @ quantum), the task switch is performed
    cmp r1, #10
    bne end_of_isr

    @ The ticks counter is reset
    mov r1, #0
    str r1, [r0]

    @ The PendSV handler is triggered
    ldr r0, =IRQ_CTRL_REGISTER
    ldr r1, =PEND_SV_BIT
    ldr r0, [r0]
    ldr r1, [r1]
    str r1, [r0]

end_of_isr:
    bx lr
