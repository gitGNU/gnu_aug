/* -*- c++ -*- */
/* Automatically generated by augidl */
/* at Wed Feb 06 06:43:01 GMT Standard Time 2008 */

#ifndef AUGCTX_LOG_H
#define AUGCTX_LOG_H

#include "augabi.h"

#if defined(__cplusplus)

#include "augabipp.hpp"

namespace aug {
    template <typename T>
    struct object_traits;
}

#endif /* __cplusplus */
#include "augctx/mpool.h"
#include <stdarg.h>

/**
 * @defgroup aug_log aug_log
 *
 * @ingroup Object
 *
 * @{
 */

AUG_INTERFACE(aug_log);

struct aug_logvtbl {
    AUG_VTBL(aug_log);
    int (*vprint_)(aug_log*, int, const char*, va_list);
};

#define aug_vprintlog(this_, level, format, args) \
    (this_)->vtbl_->vprint_(this_, level, format, args)

/** @} */

#if defined(__cplusplus)
namespace aug {
    template <>
    struct object_traits<aug_log> {
        typedef aug_logvtbl vtbl;
        static const char*
        id() AUG_NOTHROW
        {
            return aug_logid;
        }
    };
}

namespace aug {

    typedef aug::obref<aug_log> logref;

    inline int
    vprintlog(aug::obref<aug_log> this_, int level, const char* format, va_list args) AUG_NOTHROW
    {
        return this_.get()->vtbl_->vprint_(this_.get(), level, format, args);
    }

    template <typename T>
    class log {

        aug_log log_;

        log(const log&);

        log&
        operator =(const log&);

        static void*
        cast_(aug_log* this_, const char* id) AUG_NOTHROW
        {
            return aug::incget(static_cast<T*>(this_->impl_)->cast_(id));
        }
        static void
        retain_(aug_log* this_) AUG_NOTHROW
        {
            static_cast<T*>(this_->impl_)->retain_();
        }
        static void
        release_(aug_log* this_) AUG_NOTHROW
        {
            static_cast<T*>(this_->impl_)->release_();
        }
        static int
        vprint_(aug_log* this_, int level, const char* format, va_list args) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->vprintlog_(level, format, args);
        }
        static const aug_logvtbl*
        vtbl()
        {
            static const aug_logvtbl local = {
                cast_,
                retain_,
                release_,
                vprint_
            };
            return &local;
        }
    public:
        explicit
        log(T* impl = 0)
        {
            this->log_.vtbl_ = this->vtbl();
            this->log_.impl_ = impl;
        }
        void
        reset(T* impl)
        {
            this->log_.impl_ = impl;
        }
        aug_log*
        get() AUG_NOTHROW
        {
            return &this->log_;
        }
        operator aug::obref<aug_log>() AUG_NOTHROW
        {
            return this->get();
        }
    };

    template <typename T>
    class basic_log {
        log<basic_log<T> > log_;
        T impl_;
        int refs_;
        explicit
        basic_log(const T& impl)
            : impl_(impl),
              refs_(1)
        {
            this->log_.reset(this);
        }
    public:
        aug::smartob<aug_object>
        cast_(const char* id) AUG_NOTHROW
        {
            if (aug::equalid<aug_object>(id) || aug::equalid<aug_log>(id))
                return aug::object_retain<aug_object>(this->log_);
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
        int
        vprintlog_(int level, const char* format, va_list args) AUG_NOTHROW
        {
            return this->impl_.vprintlog_(level, format, args);
        }
        static aug::smartob<aug_log>
        create(const T& impl = T())
        {
            basic_log* ptr(new basic_log(impl));
            return aug::object_attach<aug_log>(ptr->log_);
        }
    };

    template <typename T>
    class scoped_log {
        log<scoped_log<T> > log_;
        T impl_;
    public:
        explicit
        scoped_log(const T& impl = T())
            : impl_(impl)
        {
            this->log_.reset(this);
        }
        aug::smartob<aug_object>
        cast_(const char* id) AUG_NOTHROW
        {
            if (aug::equalid<aug_object>(id) || aug::equalid<aug_log>(id))
                return aug::object_retain<aug_object>(this->log_);
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
        int
        vprintlog_(int level, const char* format, va_list args) AUG_NOTHROW
        {
            return this->impl_.vprintlog_(level, format, args);
        }
        aug_object*
        base() AUG_NOTHROW
        {
            return aug::obref<aug_object>(this->log_).get();
        }
        aug_log*
        get() AUG_NOTHROW
        {
            return this->log_.get();
        }
        operator aug::obref<aug_log>() AUG_NOTHROW
        {
            return this->log_;
        }
    };
}
#endif /* __cplusplus */
/**
 * Create a standard logger.
 *
 * @param mpool The memory pool from which memory will be allocated.
 *
 * @return An interface to the newly created log, or NULL on error.
 */

AUGCTX_API aug_log*
aug_createstdlog(aug_mpool* mpool);

#endif /* AUGCTX_LOG_H */
