/* Copyright (c) 2004-2006, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#ifndef AUGNETPP_INET_HPP
#define AUGNETPP_INET_HPP

#include "augnetpp/config.hpp"

#include "augsyspp/exception.hpp"
#include "augsyspp/smartfd.hpp"

#include "augnet/inet.h"

namespace aug {

    inline smartfd
    tcplisten(const struct sockaddr& addr)
    {
        smartfd sfd(smartfd::attach(aug_tcplisten(&addr)));
        if (null == sfd)
            throwerrinfo("aug_tcplisten() failed");

        return sfd;
    }

    inline void
    setnodelay(fdref ref, bool on)
    {
        int value(on ? 1 : 0);
        if (-1 == aug_setnodelay(ref.get(), value))
            throwerrinfo("aug_setnodelay() failed");
    }

    inline struct sockaddr&
    parseinet(union aug_sockunion& dst, const char* src)
    {
        if (!aug_parseinet(&dst, src))
            throwerrinfo("aug_parseinet() failed");
        return dst.sa_;
    }
}

#endif // AUGNETPP_INET_HPP
