.globl _enable_irq
.globl _stackPointerValue
.globl _cpsrValue
.globl _cpsrStore
.globl _spsrValue
.globl _spsrStore
.globl _systemStackPointerValue
.globl _systemLinkRegisterValue
.globl _systemLinkRegisterSet
.globl _systemStackPointerSet

_systemStackPointerSet:
    push {r1, r2}
    mrs r1, cpsr // current mode
    and r1, r1, #0xDF
    mov r2, #0xDF
    msr cpsr_c, r2 // system mode
    mov sp, r0
    msr cpsr_c, r1
    pop {r1, r2}
    bx lr

_systemLinkRegisterSet:
    push {r1, r2}
    mrs r1, cpsr // current mode
    and r1, r1, #0xDF
    mov r2, #0xDF
    msr cpsr_c, r2 // system mode
    mov lr, r0
    msr cpsr_c, r1
    pop {r1, r2}
    bx lr

_systemLinkRegisterValue:
    push {r1, r2}
    mrs r1, cpsr // current mode
    and r1, r1, #0xDF
    mov r2, #0xDF
    msr cpsr_c, r2 // system mode
    mov r0, lr
    msr cpsr_c, r1
    pop {r1, r2}
    bx lr

_systemStackPointerValue:
    push {r1, r2}
    mrs r1, cpsr // current mode
    and r1, r1, #0xDF
    mov r2, #0xDF
    msr cpsr_c, r2 // system mode
    mov r0, sp
    msr cpsr_c, r1
    pop {r1, r2}
    bx lr


_cpsrStore:
    msr cpsr,r0
    bx  lr

_cpsrValue:
    mrs r0,cpsr
    bx lr

_spsrValue:
    mrs r0,spsr
    bx lr

_spsrStore:
    msr spsr,r0
    bx  lr

_stackPointerValue:
    mov r0,sp
    bx  lr

_enable_irq:
    mrs r0,cpsr
    bic r0,r0,#0x80
    msr cpsr_c,r0
    bx lr