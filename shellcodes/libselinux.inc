.equ O_RDONLY,              00000000
.equ O_WRONLY,              00000001
.equ O_RDWR,                00000002
.equ O_CREAT,               00000100
.equ O_EXCL,                00000200

.equ SYS_setxattr,          5
.equ SYS_fsetxattr,         7
.equ SYS_mkdirat,           34
.equ SYS_faccessat,         48
.equ SYS_fchmod,            52
.equ SYS_fchown,            55
.equ SYS_openat,            56
.equ SYS_close,             57
.equ SYS_read,              63
.equ SYS_write,             64
.equ SYS_exit,              93
.equ SYS_umask,             166
.equ SYS_getpid,            172
.equ SYS_gettid,            178
.equ SYS_clone,             220
.equ SYS_execve,            221
.equ SYS_mmap,              222

.equ MAP_SHARED,            0x1
.equ MAP_PRIVATE,           0x2
.equ PROT_READ,             0x1
.equ PROT_WRITE,            0x2
.equ PROT_EXEC,             0x4

.equ SIGCHLD,               17
.equ CLONE_CHILD_CLEARTID,  0x00200000
.equ CLONE_CHILD_SETTID,    0x01000000

.equ pwn_path,              0x0ba
.equ checkpoint,            0x5a7

.macro movl Wn, imm
  movz    \Wn,  \imm & 0xFFFF
  movk    \Wn, (\imm >> 16) & 0xFFFF, lsl 16
.endm
