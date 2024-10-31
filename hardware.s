.syntax unified

.global write
.global read
.global nop
.global __reset

.section .vectors
.word __stack
.word __reset

.text

.thumb_func
write:
    str r1, [r0]
    bx lr

.thumb_func
read:
    ldr r1, [r0]
    mov r0, r1
    bx lr

.thumb_func
nop:
    nop
    bx lr

.thumb_func
__reset:
    b main


