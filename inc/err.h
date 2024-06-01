#ifndef ERR_H
#define ERR_H

extern void syserr(const char* fmt, ...);

extern void fatal(const char* fmt, ...);

#define SYSCALL_WITH_CHECK(call) \
    if ((call) == -1)            \
        syserr("" #call "\n");

#endif  // !ERR_H
