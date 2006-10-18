/* Copyright (c) 2004-2006, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#include "augsys/errinfo.h"

#include <io.h>
#include <winsock2.h>

AUGSYS_API int
aug_filesize(int fd, size_t* size)
{
    DWORD low, high;
    intptr_t file;

    if (-1 == (file = _get_osfhandle(fd))) {
        aug_seterrinfo(__FILE__, __LINE__, AUG_SRCLOCAL, AUG_EINVAL,
                       AUG_MSG("invalid file descriptor"));
        return -1;
    }

    low = GetFileSize((HANDLE)file, &high);

    if (-1 == low && NO_ERROR != GetLastError()) {
        aug_setwin32errinfo(__FILE__, __LINE__, GetLastError());
        return -1;
    }

    if (high) {
        aug_seterrinfo(__FILE__, __LINE__, AUG_SRCLOCAL, AUG_EINVAL,
                       AUG_MSG("file too large"));
        return -1;
    }

    *size = low;
    return 0;
}

AUGSYS_API long
aug_rand(void)
{
    return (long)rand();
}

AUGSYS_API void
aug_srand(unsigned seed)
{
    srand(seed);
}

AUGSYS_API unsigned
aug_threadid(void)
{
#if !defined(_MT)
    return 0;
#else /* _MT */
    return (unsigned)GetCurrentThreadId();
#endif /* _MT */
}
