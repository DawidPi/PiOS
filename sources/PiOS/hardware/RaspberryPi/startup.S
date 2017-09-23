.section .start

.globl _start
_start:
    ldr pc,reset_handler ;@ jump to reset + create interrupt table
    ldr pc,undefined_handler
    ldr pc,swi_handler
    ldr pc,prefetch_handler
    ldr pc,data_handler
    ldr pc,hyp_handler
    ldr pc,irq_handler
    ldr pc,fiq_handler
reset_handler:      .word reset_interrupt
undefined_handler:  .word undefined_interrupt
swi_handler:        .word swi_interrupt
prefetch_handler:   .word prefetch_interrupt
data_handler:       .word data_interrupt
hyp_handler:        .word hyp_interrupt
irq_handler:        .word irq_interrupt
fiq_handler:        .word fiq_interrupt

reset_interrupt:
    mov r0,#0x10000 ;@ copy interrupt table to the beginning (0x0000)
    mov r1,#0x0000
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}


    ;@ (PSR_IRQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD2
    msr cpsr_c,r0
    ldr sp,=#__irq_stack_top

    ;@ (PSR_FIQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD1
    msr cpsr_c,r0
    ldr sp,=#__system_stack_top

    ;@ (PSR_SVC_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD3
    msr cpsr_c,r0
    ldr sp,=#__svc_stack_top

    ;@ SVC MODE, IRQ ENABLED, FIQ DIS
    ;@mov r0,#0x53
    ;@msr cpsr_c, r0

    bl __startup
