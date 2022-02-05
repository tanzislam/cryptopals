#include <time.h>
#include <errno.h>
#include <signal.h>

int clock_gettime(clockid_t id, struct timespec * out)
{
    out->tv_sec = 1643622856;  // 2022-31-01 09:54:16 UTC
    out->tv_nsec = 0;
    return 0;
}

int isatty(int fildes)
{
    errno = ENOTTY;
    return 0;
}

long long llabs(long long i)
{
    return i >= 0 ? i : -i;
}

pid_t getpid(void)
{
    return 100;
}

pid_t fork(void)
{
    return 101;
}

pid_t waitpid(pid_t pid, int * stat_loc, int options)
{
    if (stat_loc) *stat_loc = 128 + SIGABRT;
    return pid;
}
