.globl reset_interrupt
.globl undefined_interrupt
.globl swi_interrupt
.globl prefetch_interrupt
.globl data_interrupt
.globl hyp_interrupt
.globl irq_interrupt
.globl fiq_interrupt

irq_interrupt:
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12}
    mov r0, lr
    mov r1,sp
    bl c_irq_interrupt
    mov lr, r0
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12}
    subs pc,lr,#0


swi_interrupt:
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12}
    mov r0,sp
    bl c_swi_interrupt
    mov lr, r0
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12}
    subs pc,lr,#0


undefined_interrupt:
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl c_undefined_interrupt
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    subs pc,lr,#4


prefetch_interrupt:
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl c_prefetch_interrupt
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    subs pc,lr,#4


data_interrupt:
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl c_data_interrupt
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    subs pc,lr,#4


hyp_interrupt:
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl c_unused_interrupt
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    subs pc,lr,#4


fiq_interrupt:
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl c_fiq_interrupt
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    subs pc,lr,#4
