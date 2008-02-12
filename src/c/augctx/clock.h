/* -*- c++ -*- */
/* Automatically generated by augidl */
/* at Tue Feb 12 08:33:01 +0000 2008 */

#ifndef AUGCTX_CLOCK_H
#define AUGCTX_CLOCK_H

#include "augabi.h"

#if defined(__cplusplus)

#include "augabipp.hpp"

namespace aug {
    template <typename T>
    struct object_traits;
}

#endif /* __cplusplus */
#include "augctx/mpool.h"
struct timeval;

/**
 * @defgroup aug_clock aug_clock
 *
 * @ingroup Object
 *
 * @{
 */

AUG_INTERFACE(aug_clock);

struct aug_clockvtbl {
    AUG_VTBL(aug_clock);
    struct timeval* (*gettimeofday_)(aug_clock*, struct timeval*);
    long (*gettimezone_)(aug_clock*);
};

#define aug_gettimeofday(this_, tv) \
    (this_)->vtbl_->gettimeofday_(this_, tv)

#define aug_gettimezone(this_) \
    (this_)->vtbl_->gettimezone_(this_)

/** @} */

#if defined(__cplusplus)
namespace aug {
    template <>
    struct object_traits<aug_clock> {
        typedef aug_clockvtbl vtbl;
        static const char*
        id() AUG_NOTHROW
        {
            return aug_clockid;
        }
    };
}

namespace aug {

    typedef aug::obref<aug_clock> clockref;

    inline struct timeval*
    gettimeofday(aug::obref<aug_clock> this_, struct timeval* tv) AUG_NOTHROW
    {
        return this_.get()->vtbl_->gettimeofday_(this_.get(), tv);
    }

    inline long
    gettimezone(aug::obref<aug_clock> this_) AUG_NOTHROW
    {
        return this_.get()->vtbl_->gettimezone_(this_.get());
    }

    template <typename T>
    class clock {

        aug_clock clock_;

        clock(const clock&);

        clock&
        operator =(const clock&);

        static void*
        cast_(aug_clock* this_, const char* id) AUG_NOTHROW
        {
            return aug::incget(static_cast<T*>(this_->impl_)->cast_(id));
        }
        static void
        retain_(aug_clock* this_) AUG_NOTHROW
        {
            static_cast<T*>(this_->impl_)->retain_();
        }
        static void
        release_(aug_clock* this_) AUG_NOTHROW
        {
            static_cast<T*>(this_->impl_)->release_();
        }
        static struct timeval*
        gettimeofday_(aug_clock* this_, struct timeval* tv) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->gettimeofday_(tv);
        }
        static long
        gettimezone_(aug_clock* this_) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->gettimezone_();
        }
        static const aug_clockvtbl*
        vtbl()
        {
            static const aug_clockvtbl local = {
                cast_,
                retain_,
                release_,
                gettimeofday_,
                gettimezone_
            };
            return &local;
        }
    public:
        explicit
        clock(T* impl = 0)
        {
            this->clock_.vtbl_ = this->vtbl();
            this->clock_.impl_ = impl;
        }
        void
        reset(T* impl)
        {
            this->clock_.impl_ = impl;
        }
        aug_clock*
        get() AUG_NOTHROW
        {
            return &this->clock_;
        }
        operator aug::obref<aug_clock>() AUG_NOTHROW
        {
            return this->get();
        }
    };

    template <typename T>
    class basic_clock {
        clock<basic_clock<T> > clock_;
        T impl_;
        int refs_;
        explicit
        basic_clock(const T& impl)
            : impl_(impl),
              refs_(1)
        {
            this->clock_.reset(this);
        }
    public:
        aug::smartob<aug_object>
        cast_(const char* id) AUG_NOTHROW
        {
            if (aug::equalid<aug_object>(id) || aug::equalid<aug_clock>(id))
                return aug::object_retain<aug_object>(this->clock_);
            return null;
        }
        void
        retain_() AUG_NOTHROW
        {
            assert(0 < this->refs_);
            ++this->refs_;
        }
        void
        release_() AUG_NOTHROW
        {
            assert(0 < this->refs_);
            if (0 == --this->refs_)
                delete this;
        }
        struct timeval*
        gettimeofday_(struct timeval* tv) AUG_NOTHROW
        {
            return this->impl_.gettimeofday_(tv);
        }
        long
        gettimezone_() AUG_NOTHROW
        {
            return this->impl_.gettimezone_();
        }
        static aug::smartob<aug_clock>
        create(const T& impl = T())
        {
            basic_clock* ptr(new basic_clock(impl));
            return aug::object_attach<aug_clock>(ptr->clock_);
        }
    };

    template <typename T>
    class scoped_clock {
        clock<scoped_clock<T> > clock_;
        T impl_;
    public:
        explicit
        scoped_clock(const T& impl = T())
            : impl_(impl)
        {
            this->clock_.reset(this);
        }
        aug::smartob<aug_object>
        cast_(const char* id) AUG_NOTHROW
        {
            if (aug::equalid<aug_object>(id) || aug::equalid<aug_clock>(id))
                return aug::object_retain<aug_object>(this->clock_);
            return null;
        }
        void
        retain_() AUG_NOTHROW
        {
        }
        void
        release_() AUG_NOTHROW
        {
        }
        struct timeval*
        gettimeofday_(struct timeval* tv) AUG_NOTHROW
        {
            return this->impl_.gettimeofday_(tv);
        }
        long
        gettimezone_() AUG_NOTHROW
        {
            return this->impl_.gettimezone_();
        }
        aug_object*
        base() AUG_NOTHROW
        {
            return aug::obref<aug_object>(this->clock_).get();
        }
        aug_clock*
        get() AUG_NOTHROW
        {
            return this->clock_.get();
        }
        operator aug::obref<aug_clock>() AUG_NOTHROW
        {
            return this->clock_;
        }
    };
}
#endif /* __cplusplus */
AUGCTX_API aug_clock*
aug_createclock(aug_mpool* mpool, long tz);

#endif /* AUGCTX_CLOCK_H */
