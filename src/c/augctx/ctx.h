/* -*- c++ -*- */
/* Automatically generated by augidl */
/* at Mon Feb 04 07:11:48 GMT Standard Time 2008 */

#ifndef AUGCTX_CTX_H
#define AUGCTX_CTX_H

#include "augabi.h"

#if defined(__cplusplus)

#include "augabipp.hpp"

namespace aug {
    template <typename T>
    struct object_traits;
}

#endif /* __cplusplus */
#include "augctx/clock.h"
#include "augctx/log.h"
#include "augctx/mpool.h"
struct aug_errinfo;

/**
 * @defgroup aug_ctx aug_ctx
 *
 * @ingroup Object
 *
 * @{
 */

AUG_INTERFACE(aug_ctx);

struct aug_ctxvtbl {
    AUG_VTBL(aug_ctx);
    void (*setlog_)(aug_ctx*, aug_log*);
    int (*setloglevel_)(aug_ctx*, int);
    int (*vwritelog_)(aug_ctx*, int, const char*, va_list);
    aug_mpool* (*getmpool_)(aug_ctx*);
    aug_clock* (*getclock_)(aug_ctx*);
    aug_log* (*getlog_)(aug_ctx*);
    int (*getloglevel_)(aug_ctx*);
    struct aug_errinfo* (*geterrinfo_)(aug_ctx*);
};

#define aug_setlog(this_, log) \
    (this_)->vtbl_->setlog_(this_, log)

#define aug_setloglevel(this_, level) \
    (this_)->vtbl_->setloglevel_(this_, level)

#define aug_vwritelog(this_, level, format, args) \
    (this_)->vtbl_->vwritelog_(this_, level, format, args)

#define aug_getmpool(this_) \
    (this_)->vtbl_->getmpool_(this_)

#define aug_getclock(this_) \
    (this_)->vtbl_->getclock_(this_)

#define aug_getlog(this_) \
    (this_)->vtbl_->getlog_(this_)

#define aug_getloglevel(this_) \
    (this_)->vtbl_->getloglevel_(this_)

#define aug_geterrinfo(this_) \
    (this_)->vtbl_->geterrinfo_(this_)

/** @} */

#if defined(__cplusplus)
namespace aug {
    template <>
    struct object_traits<aug_ctx> {
        typedef aug_ctxvtbl vtbl;
        static const char*
        id() AUG_NOTHROW
        {
            return aug_ctxid;
        }
    };
}

namespace aug {

    typedef aug::obref<aug_ctx> ctxref;

    inline void
    setlog(aug::obref<aug_ctx> this_, aug::obref<aug_log> log) AUG_NOTHROW
    {
        this_.get()->vtbl_->setlog_(this_.get(), log.get());
    }

    inline int
    setloglevel(aug::obref<aug_ctx> this_, int level) AUG_NOTHROW
    {
        return this_.get()->vtbl_->setloglevel_(this_.get(), level);
    }

    inline int
    vwritelog(aug::obref<aug_ctx> this_, int level, const char* format, va_list args) AUG_NOTHROW
    {
        return this_.get()->vtbl_->vwritelog_(this_.get(), level, format, args);
    }

    inline aug::smartob<aug_mpool>
    getmpool(aug::obref<aug_ctx> this_) AUG_NOTHROW
    {
        return aug::object_attach<aug_mpool>(this_.get()->vtbl_->getmpool_(this_.get()));
    }

    inline aug::smartob<aug_clock>
    getclock(aug::obref<aug_ctx> this_) AUG_NOTHROW
    {
        return aug::object_attach<aug_clock>(this_.get()->vtbl_->getclock_(this_.get()));
    }

    inline aug::smartob<aug_log>
    getlog(aug::obref<aug_ctx> this_) AUG_NOTHROW
    {
        return aug::object_attach<aug_log>(this_.get()->vtbl_->getlog_(this_.get()));
    }

    inline int
    getloglevel(aug::obref<aug_ctx> this_) AUG_NOTHROW
    {
        return this_.get()->vtbl_->getloglevel_(this_.get());
    }

    inline struct aug_errinfo*
    geterrinfo(aug::obref<aug_ctx> this_) AUG_NOTHROW
    {
        return this_.get()->vtbl_->geterrinfo_(this_.get());
    }

    template <typename T>
    class ctx {

        aug_ctx ctx_;

        ctx(const ctx&);

        ctx&
        operator =(const ctx&);

        static void*
        cast_(aug_ctx* this_, const char* id) AUG_NOTHROW
        {
            return aug::incget(static_cast<T*>(this_->impl_)->cast_(id));
        }
        static void
        retain_(aug_ctx* this_) AUG_NOTHROW
        {
            static_cast<T*>(this_->impl_)->retain_();
        }
        static void
        release_(aug_ctx* this_) AUG_NOTHROW
        {
            static_cast<T*>(this_->impl_)->release_();
        }
        static void
        setlog_(aug_ctx* this_, aug_log* log) AUG_NOTHROW
        {
            static_cast<T*>(this_->impl_)->setlog_(log);
        }
        static int
        setloglevel_(aug_ctx* this_, int level) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->setloglevel_(level);
        }
        static int
        vwritelog_(aug_ctx* this_, int level, const char* format, va_list args) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->vwritelog_(level, format, args);
        }
        static aug_mpool*
        getmpool_(aug_ctx* this_) AUG_NOTHROW
        {
            return aug::incget(static_cast<T*>(this_->impl_)->getmpool_());
        }
        static aug_clock*
        getclock_(aug_ctx* this_) AUG_NOTHROW
        {
            return aug::incget(static_cast<T*>(this_->impl_)->getclock_());
        }
        static aug_log*
        getlog_(aug_ctx* this_) AUG_NOTHROW
        {
            return aug::incget(static_cast<T*>(this_->impl_)->getlog_());
        }
        static int
        getloglevel_(aug_ctx* this_) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->getloglevel_();
        }
        static struct aug_errinfo*
        geterrinfo_(aug_ctx* this_) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->geterrinfo_();
        }
        static const aug_ctxvtbl*
        vtbl()
        {
            static const aug_ctxvtbl local = {
                cast_,
                retain_,
                release_,
                setlog_,
                setloglevel_,
                vwritelog_,
                getmpool_,
                getclock_,
                getlog_,
                getloglevel_,
                geterrinfo_
            };
            return &local;
        }
    public:
        explicit
        ctx(T* impl = 0)
        {
            this->ctx_.vtbl_ = this->vtbl();
            this->ctx_.impl_ = impl;
        }
        void
        reset(T* impl)
        {
            this->ctx_.impl_ = impl;
        }
        aug_ctx*
        get() AUG_NOTHROW
        {
            return &this->ctx_;
        }
        operator aug::obref<aug_ctx>() AUG_NOTHROW
        {
            return this->get();
        }
    };

    template <typename T>
    class basic_ctx {
        ctx<basic_ctx<T> > ctx_;
        T impl_;
        int refs_;
        explicit
        basic_ctx(const T& impl)
            : impl_(impl),
              refs_(1)
        {
            this->ctx_.reset(this);
        }
    public:
        aug::smartob<aug_object>
        cast_(const char* id) AUG_NOTHROW
        {
            if (aug::equalid<aug_object>(id) || aug::equalid<aug_ctx>(id))
                return aug::object_retain<aug_object>(this->ctx_);
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
        void
        setlog_(aug::obref<aug_log> log) AUG_NOTHROW
        {
            this->impl_.setlog_(log);
        }
        int
        setloglevel_(int level) AUG_NOTHROW
        {
            return this->impl_.setloglevel_(level);
        }
        int
        vwritelog_(int level, const char* format, va_list args) AUG_NOTHROW
        {
            return this->impl_.vwritelog_(level, format, args);
        }
        aug::smartob<aug_mpool>
        getmpool_() AUG_NOTHROW
        {
            return this->impl_.getmpool_();
        }
        aug::smartob<aug_clock>
        getclock_() AUG_NOTHROW
        {
            return this->impl_.getclock_();
        }
        aug::smartob<aug_log>
        getlog_() AUG_NOTHROW
        {
            return this->impl_.getlog_();
        }
        int
        getloglevel_() AUG_NOTHROW
        {
            return this->impl_.getloglevel_();
        }
        struct aug_errinfo*
        geterrinfo_() AUG_NOTHROW
        {
            return this->impl_.geterrinfo_();
        }
        static aug::smartob<aug_ctx>
        create(const T& impl = T())
        {
            basic_ctx* ptr(new basic_ctx(impl));
            return aug::object_attach<aug_ctx>(ptr->ctx_);
        }
    };

    template <typename T>
    class scoped_ctx {
        ctx<scoped_ctx<T> > ctx_;
        T impl_;
    public:
        explicit
        scoped_ctx(const T& impl = T())
            : impl_(impl)
        {
            this->ctx_.reset(this);
        }
        aug::smartob<aug_object>
        cast_(const char* id) AUG_NOTHROW
        {
            if (aug::equalid<aug_object>(id) || aug::equalid<aug_ctx>(id))
                return aug::object_retain<aug_object>(this->ctx_);
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
        void
        setlog_(aug::obref<aug_log> log) AUG_NOTHROW
        {
            this->impl_.setlog_(log);
        }
        int
        setloglevel_(int level) AUG_NOTHROW
        {
            return this->impl_.setloglevel_(level);
        }
        int
        vwritelog_(int level, const char* format, va_list args) AUG_NOTHROW
        {
            return this->impl_.vwritelog_(level, format, args);
        }
        aug::smartob<aug_mpool>
        getmpool_() AUG_NOTHROW
        {
            return this->impl_.getmpool_();
        }
        aug::smartob<aug_clock>
        getclock_() AUG_NOTHROW
        {
            return this->impl_.getclock_();
        }
        aug::smartob<aug_log>
        getlog_() AUG_NOTHROW
        {
            return this->impl_.getlog_();
        }
        int
        getloglevel_() AUG_NOTHROW
        {
            return this->impl_.getloglevel_();
        }
        struct aug_errinfo*
        geterrinfo_() AUG_NOTHROW
        {
            return this->impl_.geterrinfo_();
        }
        aug_object*
        base() AUG_NOTHROW
        {
            return aug::obref<aug_object>(this->ctx_).get();
        }
        aug_ctx*
        get() AUG_NOTHROW
        {
            return this->ctx_.get();
        }
        operator aug::obref<aug_ctx>() AUG_NOTHROW
        {
            return this->ctx_;
        }
    };
}
#endif /* __cplusplus */
AUGCTX_API aug_ctx*
aug_createctx(aug_mpool* mpool, aug_clock* clock, aug_log* log, int level);

AUGCTX_API int
aug_loglevel(void);

AUGCTX_API int
aug_writelog(aug_ctx* ctx, int level, const char* format, ...);

#endif /* AUGCTX_CTX_H */
