/* Copyright (c) 2004-2006, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#ifndef AUGSYSPP_SMARTFD_HPP
#define AUGSYSPP_SMARTFD_HPP

#include "augsyspp/base.hpp"
#include "augsyspp/types.hpp"

#include "augsys/string.h" // aug_perror()

#include <algorithm>       // swap()

namespace aug {

    class AUGSYSPP_API smartfd {
        fdref ref_;

        smartfd(fdref ref, bool retain) NOTHROW
        : ref_(ref)
        {
            if (retain && null != ref)
                retainfd(ref.get());
        }

    public:
        ~smartfd() NOTHROW
        {
            if (null != ref_ && -1 == aug_releasefd(ref_.get()))
                aug_perror("aug_releasefd() failed");
        }

        smartfd(const null_&) NOTHROW
            : ref_(null)
        {
        }

        smartfd(const smartfd& rhs)
            : ref_(rhs.ref_)
        {
            if (null != ref_)
                retainfd(ref_.get());
        }

        smartfd&
        operator =(const smartfd& rhs)
        {
            smartfd tmp(rhs);
            swap(tmp);
            return *this;
        }

        void
        swap(smartfd& rhs) NOTHROW
        {
            std::swap(ref_, rhs.ref_);
        }

        void
        release()
        {
            if (null != ref_) {
                fdref ref(ref_);
                ref_ = null;
                releasefd(ref.get());
            }
        }

        static smartfd
        attach(int fd)
        {
            return smartfd(fd, false);
        }

        static smartfd
        retain(int fd)
        {
            return smartfd(fd, true);
        }

        operator fdref() const
        {
            return ref_;
        }

        int
        get() const
        {
            return ref_.get();
        }
    };
}

#endif // AUGSYSPP_SMARTFD_HPP
