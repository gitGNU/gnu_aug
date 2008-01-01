/* -*- c++ -*- */
/* Automatically generated by aubidl */
/* at Sat Dec 29 16:43:17 GMT Standard Time 2007 */

#ifndef AUG_EVENTOB_H
#define AUG_EVENTOB_H

#include "aub.h"

#if defined(__cplusplus)

#include "aubpp.hpp"

# if !defined(AUB_NOTHROW)
#  define AUB_NOTHROW
# endif /* !AUB_NOTHROW */

namespace aub {
    template <typename T>
    struct object_traits;
}

#endif /* __cplusplus */

/**
 * @defgroup aug_eventob aug_eventob
 *
 * @ingroup Object
 *
 * @{
 */

AUB_INTERFACE(aug_eventob);

struct aug_eventobvtbl {
    AUB_VTBL(aug_eventob);
    void (*setuser_)(aug_eventob*, aub_object*);
    const char* (*from_)(aug_eventob*);
    const char* (*to_)(aug_eventob*);
    const char* (*type_)(aug_eventob*);
    aub_object* (*user_)(aug_eventob*);
};

#define aug_seteventobuser(this_, user) \
    ((aug_eventob*)this_)->vtbl_->setuser_(this_, user)

#define aug_eventobfrom(this_) \
    ((aug_eventob*)this_)->vtbl_->from_(this_)

#define aug_eventobto(this_) \
    ((aug_eventob*)this_)->vtbl_->to_(this_)

#define aug_eventobtype(this_) \
    ((aug_eventob*)this_)->vtbl_->type_(this_)

#define aug_eventobuser(this_) \
    ((aug_eventob*)this_)->vtbl_->user_(this_)

/** @} */

#if defined(__cplusplus)
namespace aub {
    template <>
    struct object_traits<aug_eventob> {
        typedef aug_eventobvtbl vtbl;
        static const char*
        id() AUB_NOTHROW
        {
            return aug_eventobid;
        }
    };
}

namespace aug {

    typedef aub::obref<aug_eventob> eventobref;

    inline void
    seteventobuser(aub::obref<aug_eventob> this_, aub::obref<aub_object> user) AUB_NOTHROW
    {
        this_.get()->vtbl_->setuser_(this_.get(), user.get());
    }

    inline const char*
    eventobfrom(aub::obref<aug_eventob> this_) AUB_NOTHROW
    {
        return this_.get()->vtbl_->from_(this_.get());
    }

    inline const char*
    eventobto(aub::obref<aug_eventob> this_) AUB_NOTHROW
    {
        return this_.get()->vtbl_->to_(this_.get());
    }

    inline const char*
    eventobtype(aub::obref<aug_eventob> this_) AUB_NOTHROW
    {
        return this_.get()->vtbl_->type_(this_.get());
    }

    inline aub::smartob<aub_object>
    eventobuser(aub::obref<aug_eventob> this_) AUB_NOTHROW
    {
        return aub::object_attach<aub_object>(this_.get()->vtbl_->user_(this_.get()));
    }

    template <typename T>
    class eventob {

        aug_eventob eventob_;

        eventob(const eventob&);

        eventob&
        operator =(const eventob&);

        static void*
        cast_(aug_eventob* this_, const char* id) AUB_NOTHROW
        {
            return aub::incget(static_cast<T*>(this_->impl_)->cast_(id));
        }
        static int
        retain_(aug_eventob* this_) AUB_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->retain_();
        }
        static int
        release_(aug_eventob* this_) AUB_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->release_();
        }
        static void
        setuser_(aug_eventob* this_, aub_object* user) AUB_NOTHROW
        {
            static_cast<T*>(this_->impl_)->seteventobuser_(user);
        }
        static const char*
        from_(aug_eventob* this_) AUB_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->eventobfrom_();
        }
        static const char*
        to_(aug_eventob* this_) AUB_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->eventobto_();
        }
        static const char*
        type_(aug_eventob* this_) AUB_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->eventobtype_();
        }
        static aub_object*
        user_(aug_eventob* this_) AUB_NOTHROW
        {
            return aub::incget(static_cast<T*>(this_->impl_)->eventobuser_());
        }
        static const aug_eventobvtbl*
        vtbl()
        {
            static const aug_eventobvtbl local = {
                cast_,
                retain_,
                release_,
                setuser_,
                from_,
                to_,
                type_,
                user_
            };
            return &local;
        }
    public:
        explicit
        eventob(T* impl = 0)
        {
            this->eventob_.vtbl_ = this->vtbl();
            this->eventob_.impl_ = impl;
        }
        void
        reset(T* impl)
        {
            this->eventob_.impl_ = impl;
        }
        aug_eventob*
        get() AUB_NOTHROW
        {
            return &this->eventob_;
        }
        operator aub::obref<aug_eventob>() AUB_NOTHROW
        {
            return this->get();
        }
    };

    template <typename T>
    class basic_eventob {
        eventob<basic_eventob<T> > eventob_;
        T impl_;
        int refs_;
        explicit
        basic_eventob(const T& impl)
            : impl_(impl),
              refs_(1)
        {
            this->eventob_.reset(this);
        }
    public:
        aub::smartob<aub_object>
        cast_(const char* id) AUB_NOTHROW
        {
            if (aub::equalid<aub_object>(id) || aub::equalid<aug_eventob>(id))
                return aub::object_retain<aub_object>(this->eventob_);
            return null;
        }
        int
        retain_() AUB_NOTHROW
        {
            assert(0 < this->refs_);
            ++this->refs_;
            return 0;
        }
        int
        release_() AUB_NOTHROW
        {
            assert(0 < this->refs_);
            if (0 == --this->refs_)
                delete this;
            return 0;
        }
        void
        seteventobuser_(aub::obref<aub_object> user) AUB_NOTHROW
        {
            this->impl_.seteventobuser_(user);
        }
        const char*
        eventobfrom_() AUB_NOTHROW
        {
            return this->impl_.eventobfrom_();
        }
        const char*
        eventobto_() AUB_NOTHROW
        {
            return this->impl_.eventobto_();
        }
        const char*
        eventobtype_() AUB_NOTHROW
        {
            return this->impl_.eventobtype_();
        }
        aub::smartob<aub_object>
        eventobuser_() AUB_NOTHROW
        {
            return this->impl_.eventobuser_();
        }
        static aub::smartob<aug_eventob>
        create(const T& impl = T())
        {
            basic_eventob* ptr(new basic_eventob(impl));
            return aub::object_attach<aug_eventob>(ptr->eventob_);
        }
    };

    template <typename T>
    class scoped_eventob {
        eventob<scoped_eventob<T> > eventob_;
        T impl_;
    public:
        explicit
        scoped_eventob(const T& impl = T())
            : impl_(impl)
        {
            this->eventob_.reset(this);
        }
        aub::smartob<aub_object>
        cast_(const char* id) AUB_NOTHROW
        {
            if (aub::equalid<aub_object>(id) || aub::equalid<aug_eventob>(id))
                return aub::object_retain<aub_object>(this->eventob_);
            return null;
        }
        int
        retain_() AUB_NOTHROW
        {
            return 0;
        }
        int
        release_() AUB_NOTHROW
        {
            return 0;
        }
        void
        seteventobuser_(aub::obref<aub_object> user) AUB_NOTHROW
        {
            this->impl_.seteventobuser_(user);
        }
        const char*
        eventobfrom_() AUB_NOTHROW
        {
            return this->impl_.eventobfrom_();
        }
        const char*
        eventobto_() AUB_NOTHROW
        {
            return this->impl_.eventobto_();
        }
        const char*
        eventobtype_() AUB_NOTHROW
        {
            return this->impl_.eventobtype_();
        }
        aub::smartob<aub_object>
        eventobuser_() AUB_NOTHROW
        {
            return this->impl_.eventobuser_();
        }
        aub_object*
        base() AUB_NOTHROW
        {
            return aub::obref<aub_object>(this->eventob_).get();
        }
        aug_eventob*
        get() AUB_NOTHROW
        {
            return this->eventob_.get();
        }
        operator aub::obref<aug_eventob>() AUB_NOTHROW
        {
            return this->eventob_;
        }
    };
}
#endif /* __cplusplus */

#endif /* AUG_EVENTOB_H */
