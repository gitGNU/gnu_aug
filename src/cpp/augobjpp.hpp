/* Copyright (c) 2004-2007, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#ifndef AUGOBJPP_HPP
#define AUGOBJPP_HPP

#include "augobj.h"

#include <algorithm> // swap()

#if !defined(AUG_NOTHROW)
# define AUG_NOTHROW throw()
#endif // !AUG_NOTHROW

struct null_;

namespace aug {

    template <typename T>
    class smartobj;

    template <>
    class smartobj<aug_object> {

        aug_object* obj_;

        smartobj(aug_object* obj, bool incref) AUG_NOTHROW
            : obj_(obj)
        {
            if (obj && incref)
                aug_incref(obj);
        }

    public:
        ~smartobj() AUG_NOTHROW
        {
            if (obj_)
                aug_decref(obj_);
        }

        smartobj(const null_&) AUG_NOTHROW
           : obj_(0)
        {
        }

        smartobj(const smartobj& rhs)
            : obj_(rhs.obj_)
        {
            if (obj_)
                aug_incref(obj_);
        }

        smartobj&
        operator =(const null_&) AUG_NOTHROW
        {
            *this = smartobj::incref(0);
            return *this;
        }

        smartobj&
        operator =(const smartobj& rhs)
        {
            smartobj tmp(rhs);
            swap(tmp);
            return *this;
        }

        void
        swap(smartobj& rhs) AUG_NOTHROW
        {
            std::swap(obj_, rhs.obj_);
        }

        void
        release()
        {
            if (obj_) {
                aug_object* obj(obj_);
                obj_ = 0;
                aug_decref(obj);
            }
        }

        static smartobj
        attach(aug_object* obj)
        {
            return smartobj(obj, false);
        }

        static smartobj
        incref(aug_object* obj)
        {
            return smartobj(obj, true);
        }

        aug_object*
        get() const
        {
            return obj_;
        }

        operator aug_object*() const
        {
            return get();
        }
    };

    template <typename T>
    class smartobj {

        T* obj_;

        smartobj(T* obj, bool incref) AUG_NOTHROW
            : obj_(obj)
        {
            if (obj && incref)
                aug_incref(obj);
        }

    public:
        ~smartobj() AUG_NOTHROW
        {
            if (obj_)
                aug_decref(obj_);
        }

        smartobj(const null_&) AUG_NOTHROW
           : obj_(0)
        {
        }

        smartobj(const smartobj& rhs)
            : obj_(rhs.obj_)
        {
            if (obj_)
                aug_incref(obj_);
        }

        smartobj&
        operator =(const null_&) AUG_NOTHROW
        {
            *this = smartobj::incref(0);
            return *this;
        }

        smartobj&
        operator =(const smartobj& rhs)
        {
            smartobj tmp(rhs);
            swap(tmp);
            return *this;
        }

        void
        swap(smartobj& rhs) AUG_NOTHROW
        {
            std::swap(obj_, rhs.obj_);
        }

        void
        release()
        {
            if (obj_) {
                T* obj(obj_);
                obj_ = 0;
                aug_decref(obj);
            }
        }

        static smartobj
        attach(T* obj)
        {
            return smartobj(obj, false);
        }

        static smartobj
        incref(T* obj)
        {
            return smartobj(obj, true);
        }

        T*
        get() const
        {
            return obj_;
        }

        operator T*() const
        {
            return get();
        }

        operator aug_object*() const
        {
            return reinterpret_cast<aug_object*>(get());
        }
    };

    template <typename T>
    struct object_traits;

    inline bool
    equalid(const char* lhs, const char* rhs)
    {
        return AUG_EQUALID(lhs, rhs);
    }

    template <typename T>
    bool
    equalid(const char* id)
    {
        return equalid(object_traits<T>::id(), id);
    }

    template <>
    struct object_traits<aug_object> {
        static const char*
        id()
        {
            return aug_objectid;
        }
    };

    template <typename T>
    smartobj<T>
    object_attach(T* obj)
    {
        return smartobj<T>::attach(obj);
    }

    template <typename T, typename U>
    smartobj<T>
    object_cast(U* obj)
    {
        return smartobj<T>::attach
            (static_cast<T*>(obj->vtbl_->cast_(obj, object_traits<T>::id())));
    }

    template <typename T, typename U>
    smartobj<T>
    object_cast(const smartobj<U>& sobj)
    {
        return object_cast<T, U>(static_cast<U*>(sobj));
    }

    template <typename T>
    int
    incref(T* obj)
    {
        return obj->vtbl_->incref_(obj);
    }

    template <typename T>
    int
    decref(T* obj)
    {
        return obj->vtbl_->decref_(obj);
    }

    template <>
    struct object_traits<aug_blob> {
        static const char*
        id()
        {
            return aug_blobid;
        }
    };

    inline const void*
    blobdata(aug_blob* obj, size_t* size) AUG_NOTHROW
    {
        return obj->vtbl_->data_(obj, size);
    }

    template <typename T>
    class blob_base {

        static void*
        cast_(aug_blob* obj, const char* id) AUG_NOTHROW
        {
            T* impl = static_cast<T*>(obj->impl_);
            return impl->cast(id);
        }

        static int
        incref_(aug_blob* obj) AUG_NOTHROW
        {
            T* impl = static_cast<T*>(obj->impl_);
            return impl->incref();
        }

        static int
        decref_(aug_blob* obj) AUG_NOTHROW
        {
            T* impl = static_cast<T*>(obj->impl_);
            return impl->decref();
        }

        static const void*
        data_(aug_blob* obj, size_t* size) AUG_NOTHROW
        {
            T* impl = static_cast<T*>(obj->impl_);
            return impl->blobdata(size);
        }

    protected:
        ~blob_base() AUG_NOTHROW
        {
        }
        static const aug_blobvtbl*
        blobvtbl()
        {
            static const aug_blobvtbl local = {
                cast_,
                incref_,
                decref_,
                data_
            };
            return &local;
        }
    };
}

template <typename T>
bool
isnull(const aug::smartobj<T>& sobj)
{
    return 0 == sobj.get();
}

#endif // AUGOBJPP_HPP
