//
// Created by igio90 on 23/03/22.
//

#ifndef DIRTYPIPEZ_SYSCALLS_H
#define DIRTYPIPEZ_SYSCALLS_H

#include "syscall_arch.h"

#include <asm/unistd.h>
#include <sys/types.h>
#include <asm-generic/signal.h>
#include <linux/fcntl.h>


__attribute__((always_inline))
static inline int sys_close(int __fd) {
    return (int)__syscall1(__NR_close, __fd);
}

__attribute__((always_inline))
static inline void sys_execve(const char * filename, const char *const *argv, const char *const *envp) {
    __syscall3(__NR_execve, (long) filename, (long) argv, (long) envp);
}

__attribute__((always_inline))
static inline void sys_exit(int __status) {
    __syscall1(__NR_exit, __status);
}

__attribute__((always_inline))
static inline int sys_faccessat(int dfd, const char __user * filename, int mode, int flags) {
return (int)__syscall4(__NR_faccessat, dfd, (long) filename, mode, flags);
}

__attribute__((always_inline))
static inline pid_t sys_fork() {
#ifdef __NR_clone
    return __syscall5(__NR_clone, SIGCHLD, 0, 0, 0, 0);
#elif defined(__NR_fork)
    return __syscall0(__NR_fork);
#endif
    return 0;
}

__attribute__((always_inline))
static inline pid_t sys_getpid() {
    return (int)__syscall0(__NR_getpid);
}

__attribute__((always_inline))
static inline pid_t sys_getppid() {
    return (int)__syscall0(__NR_getppid);
}

__attribute__((always_inline))
static inline pid_t sys_gettid() {
    return (int)__syscall0(__NR_gettid);
}

__attribute__((always_inline))
static inline int sys_kill(pid_t pid, int sig) {
    return (int)__syscall2(__NR_kill, (long) pid, sig);
}

__attribute__((always_inline))
static inline off_t sys_lseek(int __fd, off_t __offset, int __whence) {
    return __syscall3(__NR_lseek, __fd, __offset, __whence);
}

__attribute__((always_inline))
static inline int sys_mprotect(void * addr, size_t len, int prot) {
    return (int) __syscall3(__NR_mprotect, (long) addr, len, prot);
}

__attribute__((always_inline))
static inline int sys_nanosleep(const struct timespec * __request, struct timespec* __remainder) {
    return (int)__syscall2(__NR_nanosleep, (long)__request, (long)__remainder);
}

__attribute__((always_inline))
static inline int  sys_openat(const char * __path, int __flags, int __mode) {
#ifdef __NR_openat
    return (int)__syscall4(__NR_openat, AT_FDCWD, (long) __path, __flags, __mode);
#elif defined(__NR_open)
    return (int) __syscall3(__NR_open, (long) __path, __flags, __mode);
#else
    return 0;
#endif
}

__attribute__((always_inline))
static inline int sys_ptrace(int request, pid_t pid, void * addr, void * data) {
    return (int) __syscall4(__NR_ptrace, request, (long) pid, (long) addr, (long) data);
}

__attribute__((always_inline))
static inline ssize_t sys_read(int __fd, void * __buf, size_t __count) {
    return __syscall3(__NR_read, __fd, (long)__buf, (long)__count);
}

__attribute__((always_inline))
static inline ssize_t sys_socket(int domain, int type, int protocol) {
    return __syscall3(__NR_socket, domain, type, protocol);
}

__attribute__((always_inline))
static inline ssize_t sys_readlinkat(int __dir_fd, const char * __path, char * __buf, size_t __buf_size) {
    return __syscall4(__NR_readlinkat, __dir_fd, (long)__path, (long)__buf, (long)__buf_size);
}

__attribute__((always_inline))
static inline int sys_tgkill(int __tgid, int __tid, int __signal) {
    return (int)__syscall3(__NR_tgkill, __tgid, __tid, __signal);
}

__attribute__((always_inline))
static inline int sys_tkill(int __tid, int __signal) {
    return (int)__syscall2(__NR_tkill, __tid, __signal);
}

__attribute__((always_inline))
static inline int sys_unlinkat(int dirfd, const char * pathname, int flags) {
    return (int)__syscall3(__NR_unlinkat, dirfd, (long) pathname, flags);
}

__attribute__((always_inline))
static inline size_t sys_write(int fd, const void * buf, size_t count) {
    return (int) __syscall3(__NR_write, fd, (long) buf, count);
}

#endif //DIRTYPIPEZ_SYSCALLS_H
