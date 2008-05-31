/* Copyright (c) 2004-2007, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#ifndef AUGNETPP_OBJECT_HPP
#define AUGNETPP_OBJECT_HPP

#include "augnetpp/config.hpp"

#include "augsyspp/exception.hpp"

#include "augnet/object.h"

#include "augsys/muxer.h"

#include "augext/chan.h"
#include "augext/mpool.h"

namespace aug {

    inline chanptr
    createclient(mpoolref mpool, const char* host, const char* serv,
                 aug_muxer_t muxer, struct ssl_st* ssl = 0)
    {
        return object_attach<aug_chan>
            (verify(aug_createclient(mpool.get(), host, serv, muxer, ssl)));
    }

    inline chanptr
    createserver(mpoolref mpool, aug_muxer_t muxer, sdref sd,
                 struct ssl_st* ssl = 0)
    {
        return object_attach<aug_chan>
            (verify(aug_createserver(mpool.get(), muxer, sd.get(), ssl)));
    }

    inline chanptr
    createplain(mpoolref mpool, unsigned id, aug_muxer_t muxer, sdref sd,
                unsigned short mask)
    {
        return object_attach<aug_chan>
            (verify(aug_createplain(mpool.get(), id, muxer, sd.get(), mask)));
    }
}

#endif // AUGNETPP_OBJECT_HPP
