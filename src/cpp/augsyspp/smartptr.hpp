/* Copyright (c) 2004-2006, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#ifndef AUGSYSPP_SMARTPTR_HPP
#define AUGSYSPP_SMARTPTR_HPP

#include "augsyspp/types.hpp"

#include <algorithm> // swap()

/**
   A simple shared pointer implementation.  Where available, boost's
   shared_ptr should be preferred.  Note: the reference count implementation
   is not thread-safe.
*/

namespace aug {

    template <typename T>
    class smartptr {
        T* ptr_;
        unsigned* refs_;

        void
        release()
        {
            if (ptr_ && 0 == --*refs_) {
                delete ptr_;
                delete refs_;
                ptr_ = 0;
                refs_ = 0;
            }
        }
        void
        retain()
        {
            if (ptr_)
                ++*refs_;
        }

    public:
        ~smartptr() AUG_NOTHROW
        {
            release();
        }
        smartptr(const null_&) AUG_NOTHROW
        : ptr_(0),
            refs_(0)
        {
        }
        explicit
        smartptr(T* ptr = 0)
            : ptr_(ptr),
              refs_(ptr ? new unsigned(1) : 0)
        {
        }
        smartptr(const smartptr& rhs) AUG_NOTHROW
        : ptr_(rhs.ptr_),
            refs_(rhs.refs_)
        {
            retain();
        }
        smartptr&
        operator =(const null_&) AUG_NOTHROW
        {
            release();
            return *this;
        }
        smartptr&
        operator =(const smartptr& rhs) AUG_NOTHROW
        {
            smartptr ptr(rhs);
            swap(ptr);
            return *this;
        }
        void
        swap(smartptr& rhs) AUG_NOTHROW
        {
            std::swap(ptr_, rhs.ptr_);
            std::swap(refs_, rhs.refs_);
        }
        unsigned
        refs() const
        {
            return *refs_;
        }
        T*
        get() const AUG_NOTHROW
        {
            return ptr_;
        }
        T&
        operator*() const AUG_NOTHROW
        {
            return *ptr_;
        }
        T*
        operator->() const AUG_NOTHROW
        {
            return ptr_;
        }
    };
}

template <typename T>
bool
isnull(const aug::smartptr<T>& ptr)
{
    return 0 == ptr.get();
}

#endif // AUGSYSPP_SMARTPTR_HPP