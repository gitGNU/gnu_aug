/* Copyright (c) 2004-2006, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#ifndef AUGSYSPP_UNISTD_HPP
#define AUGSYSPP_UNISTD_HPP

#include "augsyspp/exception.hpp"
#include "augsyspp/smartfd.hpp"

#include "augsys/unistd.h"

namespace aug {

    inline void
    close(int fd)
    {
        if (-1 == aug_close(fd))
            throwerrinfo("aug_close() failed");
    }

    inline std::pair<smartfd, smartfd>
    pipe()
    {
        int fds[2];
        if (-1 == aug_pipe(fds))
            throwerrinfo("aug_pipe() failed");

        return std::make_pair(smartfd::attach(fds[0]),
                              smartfd::attach(fds[1]));
    }

    inline size_t
    read(fdref ref, void* buf, size_t size)
    {
        ssize_t ret(aug_read(ref.get(), buf, size));
        if (-1 == ret)
            throwerrinfo("aug_read() failed");
        return ret;
    }

    inline size_t
    write(fdref ref, const void* buf, size_t size)
    {
        ssize_t ret(aug_write(ref.get(), buf, size));
        if (-1 == ret)
            throwerrinfo("aug_write() failed");
        return ret;
    }

    inline void
    sleep(unsigned int ms)
    {
        return aug_sleep(ms);
    }
}

#endif // AUGSYSPP_UNISTD_HPP
