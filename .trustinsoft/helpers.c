#include <time.h>
#include <errno.h>

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
