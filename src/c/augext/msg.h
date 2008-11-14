/* -*- c++ -*- */
/* Automatically generated by augidl */
/* at Fri Nov 14 07:00:01 GMT Standard Time 2008 */

#ifndef AUGEXT_MSG_H
#define AUGEXT_MSG_H

#include "augabi.h"

#if defined(__cplusplus)

#include "augabipp.hpp"

namespace aug {
    template <typename T>
    struct object_traits;
}

#endif /* __cplusplus */

/* Includes. */


/* Interface declarations. */

AUG_INTERFACE(aug_msg);

/* Interface definitions. */

/**
 * @defgroup aug_msg aug_msg
 *
 * @ingroup Object
 *
 * @{
 */

struct aug_msgvtbl {
    AUG_VTBL(aug_msg);
    void (*setpayload_)(aug_msg*, aug_object*);
    const char* (*getfrom_)(aug_msg*);
    const char* (*getto_)(aug_msg*);
    const char* (*gettype_)(aug_msg*);
    aug_object* (*getpayload_)(aug_msg*);
};

#define aug_setmsgpayload(this_, payload) \
    (this_)->vtbl_->setpayload_(this_, payload)

#define aug_getmsgfrom(this_) \
    (this_)->vtbl_->getfrom_(this_)

#define aug_getmsgto(this_) \
    (this_)->vtbl_->getto_(this_)

#define aug_getmsgtype(this_) \
    (this_)->vtbl_->gettype_(this_)

#define aug_getmsgpayload(this_) \
    (this_)->vtbl_->getpayload_(this_)

/** @} */

#if defined(__cplusplus)
namespace aug {
    template <>
    struct object_traits<aug_msg> {
        typedef aug_msgvtbl vtbl;
        static const char*
        id() AUG_NOTHROW
        {
            return aug_msgid;
        }
    };
}

namespace aug {

    template <typename T>
    class msg {

        aug_msg msg_;

        msg(const msg&);

        msg&
        operator =(const msg&);

        static void*
        cast_(aug_msg* this_, const char* id) AUG_NOTHROW
        {
            return aug::retget(static_cast<T*>(this_->impl_)->cast_(id));
        }
        static void
        retain_(aug_msg* this_) AUG_NOTHROW
        {
            static_cast<T*>(this_->impl_)->retain_();
        }
        static void
        release_(aug_msg* this_) AUG_NOTHROW
        {
            static_cast<T*>(this_->impl_)->release_();
        }
        static void
        setpayload_(aug_msg* this_, aug_object* payload) AUG_NOTHROW
        {
            static_cast<T*>(this_->impl_)->setmsgpayload_(payload);
        }
        static const char*
        getfrom_(aug_msg* this_) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->getmsgfrom_();
        }
        static const char*
        getto_(aug_msg* this_) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->getmsgto_();
        }
        static const char*
        gettype_(aug_msg* this_) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->getmsgtype_();
        }
        static aug_object*
        getpayload_(aug_msg* this_) AUG_NOTHROW
        {
            return aug::retget(static_cast<T*>(this_->impl_)->getmsgpayload_());
        }
        static const aug_msgvtbl*
        vtbl()
        {
            static const aug_msgvtbl local = {
                cast_,
                retain_,
                release_,
                setpayload_,
                getfrom_,
                getto_,
                gettype_,
                getpayload_
            };
            return &local;
        }
    public:
        explicit
        msg(T* impl = 0)
        {
            this->msg_.vtbl_ = this->vtbl();
            this->msg_.impl_ = impl;
        }
        void
        reset(T* impl)
        {
            this->msg_.impl_ = impl;
        }
        aug_msg*
        get() AUG_NOTHROW
        {
            return &this->msg_;
        }
        operator aug::obref<aug_msg>() AUG_NOTHROW
        {
            return this->get();
        }
    };

    typedef aug::obref<aug_msg> msgref;
    typedef aug::smartob<aug_msg> msgptr;
}
#endif /* __cplusplus */

/* C++ definitions. */

#if defined(__cplusplus)
namespace aug {

    inline void
    setmsgpayload(aug::obref<aug_msg> this_, aug::obref<aug_object> payload) AUG_NOTHROW
    {
        this_.get()->vtbl_->setpayload_(this_.get(), payload.get());
    }

    inline const char*
    getmsgfrom(aug::obref<aug_msg> this_) AUG_NOTHROW
    {
        return this_.get()->vtbl_->getfrom_(this_.get());
    }

    inline const char*
    getmsgto(aug::obref<aug_msg> this_) AUG_NOTHROW
    {
        return this_.get()->vtbl_->getto_(this_.get());
    }

    inline const char*
    getmsgtype(aug::obref<aug_msg> this_) AUG_NOTHROW
    {
        return this_.get()->vtbl_->gettype_(this_.get());
    }

    inline aug::smartob<aug_object>
    getmsgpayload(aug::obref<aug_msg> this_) AUG_NOTHROW
    {
        return aug::object_attach<aug_object>(this_.get()->vtbl_->getpayload_(this_.get()));
    }

    template <typename T>
    class basic_msg {
        msg<basic_msg<T> > msg_;
        T impl_;
        int refs_;
        explicit
        basic_msg(const T& impl)
            : impl_(impl),
              refs_(1)
        {
            this->msg_.reset(this);
        }
    public:
        aug::smartob<aug_object>
        cast_(const char* id) AUG_NOTHROW
        {
            if (aug::equalid<aug_object>(id) || aug::equalid<aug_msg>(id))
                return aug::object_retain<aug_object>(this->msg_);
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
        setmsgpayload_(aug::obref<aug_object> payload) AUG_NOTHROW
        {
            this->impl_.setmsgpayload_(payload);
        }
        const char*
        getmsgfrom_() AUG_NOTHROW
        {
            return this->impl_.getmsgfrom_();
        }
        const char*
        getmsgto_() AUG_NOTHROW
        {
            return this->impl_.getmsgto_();
        }
        const char*
        getmsgtype_() AUG_NOTHROW
        {
            return this->impl_.getmsgtype_();
        }
        aug::smartob<aug_object>
        getmsgpayload_() AUG_NOTHROW
        {
            return this->impl_.getmsgpayload_();
        }
        static aug::smartob<aug_msg>
        create(const T& impl = T())
        {
            basic_msg* ptr(new basic_msg(impl));
            return aug::object_attach<aug_msg>(ptr->msg_);
        }
    };

    template <typename T>
    class scoped_msg {
        msg<scoped_msg<T> > msg_;
        T impl_;
    public:
        explicit
        scoped_msg(const T& impl = T())
            : impl_(impl)
        {
            this->msg_.reset(this);
        }
        aug::smartob<aug_object>
        cast_(const char* id) AUG_NOTHROW
        {
            if (aug::equalid<aug_object>(id) || aug::equalid<aug_msg>(id))
                return aug::object_retain<aug_object>(this->msg_);
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
        setmsgpayload_(aug::obref<aug_object> payload) AUG_NOTHROW
        {
            this->impl_.setmsgpayload_(payload);
        }
        const char*
        getmsgfrom_() AUG_NOTHROW
        {
            return this->impl_.getmsgfrom_();
        }
        const char*
        getmsgto_() AUG_NOTHROW
        {
            return this->impl_.getmsgto_();
        }
        const char*
        getmsgtype_() AUG_NOTHROW
        {
            return this->impl_.getmsgtype_();
        }
        aug::smartob<aug_object>
        getmsgpayload_() AUG_NOTHROW
        {
            return this->impl_.getmsgpayload_();
        }
        aug_object*
        base() AUG_NOTHROW
        {
            return aug::obref<aug_object>(this->msg_).get();
        }
        aug_msg*
        get() AUG_NOTHROW
        {
            return this->msg_.get();
        }
        operator aug::obref<aug_msg>() AUG_NOTHROW
        {
            return this->msg_;
        }
    };

    typedef aug::smartob<aug_msg> msgptr;
}
#endif /* __cplusplus */

#endif /* AUGEXT_MSG_H */
