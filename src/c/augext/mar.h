/* -*- c++ -*- */
/* Automatically generated by augidl */
/* at Tue Mar 10 18:57:30 +0000 2009 */

#ifndef AUGEXT_MAR_H
#define AUGEXT_MAR_H

#include "augabi.h"

#if defined(__cplusplus)

#include "augabipp.hpp"

namespace aug {
    template <typename T>
    struct object_traits;
}

#endif /* __cplusplus */

/* Includes. */

#include "augtypes.h"

/* Interface declarations. */

AUG_INTERFACE(aug_mar);
AUG_INTERFACE(aug_marstore);

/* Interface definitions. */

struct aug_field;

/**
 * @defgroup aug_mar aug_mar
 *
 * @ingroup Object
 *
 * @{
 */

struct aug_marvtbl {
    AUG_VTBL(aug_mar);
    aug_result (*compact_)(aug_mar*);
    aug_rint (*clear_)(aug_mar*);
    aug_result (*deln_)(aug_mar*, unsigned);
    aug_rint (*delp_)(aug_mar*, const char*);
    aug_rint (*getn_)(aug_mar*, unsigned, const void**);
    aug_rint (*getp_)(aug_mar*, const char*, const void**);
    aug_result (*get_)(aug_mar*, unsigned, struct aug_field*);
    aug_result (*putn_)(aug_mar*, unsigned, const void*, unsigned);
    aug_rint (*putp_)(aug_mar*, const char*, const void*, unsigned);
    aug_rint (*put_)(aug_mar*, const struct aug_field*);
    aug_result (*ntop_)(aug_mar*, unsigned, const char**);
    aug_rint (*pton_)(aug_mar*, const char*);
    unsigned (*getcount_)(aug_mar*);
    aug_result (*insert_)(aug_mar*, const char*);
    aug_rsize (*seek_)(aug_mar*, off_t, int);
    aug_result (*setcontent_)(aug_mar*, const void*, unsigned);
    aug_result (*sync_)(aug_mar*);
    aug_result (*truncate_)(aug_mar*, unsigned);
    aug_rsize (*write_)(aug_mar*, const void*, unsigned);
    aug_result (*extract_)(aug_mar*, const char*);
    const void* (*getcontent_)(aug_mar*, unsigned*);
    aug_rsize (*read_)(aug_mar*, void*, unsigned);
    unsigned (*getcontentsize_)(aug_mar*);
};

/**
 * Compact any unused space within message archive.
 * @param this_ The object.
 * @return See @ref TypesResult.
 */

#define aug_compactmar(this_) \
    (this_)->vtbl_->compact_(this_)

/**
 * Clear all fields within message archive.
 * @param this_ The object.
 * @return See @ref TypesResult.  The number of fields removed on success.
 */

#define aug_clearfields(this_) \
    (this_)->vtbl_->clear_(this_)

/**
 * Delete field (by ordinal) within message archive.
 * @param this_ The object.
 * @param n The ordinal position of the field.
 * @return See @ref TypesResult.  #AUG_FAILNONE if there is no matching
 * field.
 */

#define aug_delfieldn(this_, n) \
    (this_)->vtbl_->deln_(this_, n)

/**
 * Delete field (by name) within message archive.
 * @param this_ The object.
 * @param name The name of the field.
 * @return See @ref TypesResult.  The ordinal field position on success.
 * #AUG_FAILNONE if there is no matching field.
 */

#define aug_delfieldp(this_, name) \
    (this_)->vtbl_->delp_(this_, name)

/**
 * Obtain field value (by ordinal) from message archive.
 * @param this_ The object.
 * @param n The ordinal position of the field.
 * @param value The output parameter, in which, the value will be set.  Set
 * to null if if there is no matching field.
 * @return See @ref TypesResult.  The size of the field on success.
 */

#define aug_getfieldn(this_, n, value) \
    (this_)->vtbl_->getn_(this_, n, value)

/**
 * Obtain field value (by name) from message archive.
 * @param this_ The object.
 * @param name The name of the field.
 * @param value The output parameter, in which, the value will be set.  Set
 * to null if if there is no matching field.
 * @return See @ref TypesResult.  The size of the field on success.
 */

#define aug_getfieldp(this_, name, value) \
    (this_)->vtbl_->getp_(this_, name, value)

/**
 * Obtain field from message archive.
 * @param this_ The object.
 * @param n The ordinal position of the field.
 * @param field The output parameter, in which, the field will be returned.
 * The field's name and value will be set to null if if there is no
 * matching field.
 * @return See @ref TypesResult.
 */

#define aug_getfield(this_, n, field) \
    (this_)->vtbl_->get_(this_, n, field)

/**
 * Set field value (by ordinal) within message archive.
 * @param this_ The object.
 * @param n The ordinal position of an existing field.
 * @param value A pointer to the field value to be assigned.
 * @param size The size of the field value to be assigned.
 * @return See @ref TypesResult.  #AUG_FAILNONE if there is no matching
 * field.
 */

#define aug_putfieldn(this_, n, value, size) \
    (this_)->vtbl_->putn_(this_, n, value, size)

/**
 * Set field value (by name) within message archive.
 * @param this_ The object.
 * @param name The name of an existing field.
 * @param value A pointer to the field value to be assigned.
 * @param size The size of the field value to be assigned.
 * @return See @ref TypesResult.  The ordinal field position on success.
 * #AUG_FAILNONE if there is no matching field.
 */

#define aug_putfieldp(this_, name, value, size) \
    (this_)->vtbl_->putp_(this_, name, value, size)

/**
 * Set field within message archive.
 * @param this_ The object.
 * @param field A field specifying the field name and a pointer to, and
 * size of, the field value.
 * @return See @ref TypesResult.  The ordinal field position on success.
 */

#define aug_putfield(this_, field) \
    (this_)->vtbl_->put_(this_, field)

/**
 * Get field name from ordinal position in message archive.
 * @param this_ The object.
 * @param n The ordinal position of the name to be returned.
 * @param name The output parameter, in which, the name will be returned.
 * @return See @ref TypesResult.  #AUG_FAILNONE if there is no matching
 * field.
 */

#define aug_fieldntop(this_, n, name) \
    (this_)->vtbl_->ntop_(this_, n, name)

/**
 * Get ordinal position from field name in message archive.
 * @param this_ The object.
 * @param name The name of the field.
 * @return See @ref TypesResult.  The ordinal field position on success.
 * #AUG_FAILNONE if there is no matching field.
 */

#define aug_fieldpton(this_, name) \
    (this_)->vtbl_->pton_(this_, name)

/**
 * Gets the number of fields contained within message archive.
 * @param this_ The object.
 * @return The number of fields.
 */

#define aug_getfieldcount(this_) \
    (this_)->vtbl_->getcount_(this_)

/**
 * Insert file into body content of message archive.
 * @param this_ The object.
 * @param path A path to the file from which the content is read.
 * @return See @ref TypesResult.
 */

#define aug_insertmar(this_, path) \
    (this_)->vtbl_->insert_(this_, path)

/**
 * Reposition content offset within message archive.
 * @param this_ The object.
 * @param offset The offset calculated according to @a whence.
 * @param whence The @ref SeekWhence directive.
 * @return See @ref TypesResult.  If positive, the resulting location
 * offset, in bytes, from the beginning of content.
 */

#define aug_seekmar(this_, offset, whence) \
    (this_)->vtbl_->seek_(this_, offset, whence)

/**
 * Set content within message archive.
 * @param this_ The object.
 * @param cdata A pointer to the content to be copied.
 * @param size The size of the content to be copied.
 * @return See @ref TypesResult.
 */

#define aug_setcontent(this_, cdata, size) \
    (this_)->vtbl_->setcontent_(this_, cdata, size)

/**
 * Flush message archive buffers.
 */

#define aug_syncmar(this_) \
    (this_)->vtbl_->sync_(this_)

/**
 * Truncate content within message archive.
 * @param this_ The object.
 * @param size The size to which the content will be truncated.
 * @return See @ref TypesResult.
 */

#define aug_truncatemar(this_, size) \
    (this_)->vtbl_->truncate_(this_, size)

/**
 * Write content to message archive.
 * @param this_ The object.
 * @param buf The buffer from which the content will be copied.
 * @param len The number of bytes to be written.
 * @return See @ref TypesResult.  If positive, the number of bytes actually
 * written.
 */

#define aug_writemar(this_, buf, len) \
    (this_)->vtbl_->write_(this_, buf, len)

/**
 * Extract content from message archive into file.
 * @param this_ The object.
 * @param path A path to the file into which the content will be written.
 * @return See @ref TypesResult.
 */

#define aug_extractmar(this_, path) \
    (this_)->vtbl_->extract_(this_, path)

/**
 * Obtain content from message archive.
 * @param this_ The object.
 * @param size An optional output parameter, in which, the size of the
 * content is set.
 * @return A pointer to the content or null on failure.
 */

#define aug_getcontent(this_, size) \
    (this_)->vtbl_->getcontent_(this_, size)

/**
 * Read content from message archive.
 * @param this_ The object.
 * @param buf The output buffer into which the content will be copied.
 * @param len The number of bytes to be read.
 * @return See @ref TypesResult.  If positive, the number of bytes actually
 * read; zero if end-of-file.
 */

#define aug_readmar(this_, buf, len) \
    (this_)->vtbl_->read_(this_, buf, len)

/**
 * Obtain size of content within message archive.
 * @param this_ The object.
 * @return Content size.
 */

#define aug_getcontentsize(this_) \
    (this_)->vtbl_->getcontentsize_(this_)

/** @} */

#if defined(__cplusplus)
namespace aug {
    template <>
    struct object_traits<aug_mar> {
        typedef aug_marvtbl vtbl;
        static const char*
        id() AUG_NOTHROW
        {
            return aug_marid;
        }
    };
}

namespace aug {

    template <typename T>
    class mar {

        aug_mar mar_;

        mar(const mar&);

        mar&
        operator =(const mar&);

        static void*
        cast_(aug_mar* this_, const char* id) AUG_NOTHROW
        {
            return aug::retget(static_cast<T*>(this_->impl_)->cast_(id));
        }
        static void
        retain_(aug_mar* this_) AUG_NOTHROW
        {
            static_cast<T*>(this_->impl_)->retain_();
        }
        static void
        release_(aug_mar* this_) AUG_NOTHROW
        {
            static_cast<T*>(this_->impl_)->release_();
        }
        static aug_result
        compact_(aug_mar* this_) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->compactmar_();
        }
        static aug_rint
        clear_(aug_mar* this_) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->clearfields_();
        }
        static aug_result
        deln_(aug_mar* this_, unsigned n) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->delfieldn_(n);
        }
        static aug_rint
        delp_(aug_mar* this_, const char* name) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->delfieldp_(name);
        }
        static aug_rint
        getn_(aug_mar* this_, unsigned n, const void** value) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->getfieldn_(n, *value);
        }
        static aug_rint
        getp_(aug_mar* this_, const char* name, const void** value) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->getfieldp_(name, *value);
        }
        static aug_result
        get_(aug_mar* this_, unsigned n, struct aug_field* field) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->getfield_(n, *field);
        }
        static aug_result
        putn_(aug_mar* this_, unsigned n, const void* value, unsigned size) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->putfieldn_(n, value, size);
        }
        static aug_rint
        putp_(aug_mar* this_, const char* name, const void* value, unsigned size) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->putfieldp_(name, value, size);
        }
        static aug_rint
        put_(aug_mar* this_, const struct aug_field* field) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->putfield_(*field);
        }
        static aug_result
        ntop_(aug_mar* this_, unsigned n, const char** name) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->fieldntop_(n, *name);
        }
        static aug_rint
        pton_(aug_mar* this_, const char* name) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->fieldpton_(name);
        }
        static unsigned
        getcount_(aug_mar* this_) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->getfieldcount_();
        }
        static aug_result
        insert_(aug_mar* this_, const char* path) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->insertmar_(path);
        }
        static aug_rsize
        seek_(aug_mar* this_, off_t offset, int whence) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->seekmar_(offset, whence);
        }
        static aug_result
        setcontent_(aug_mar* this_, const void* cdata, unsigned size) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->setcontent_(cdata, size);
        }
        static aug_result
        sync_(aug_mar* this_) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->syncmar_();
        }
        static aug_result
        truncate_(aug_mar* this_, unsigned size) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->truncatemar_(size);
        }
        static aug_rsize
        write_(aug_mar* this_, const void* buf, unsigned len) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->writemar_(buf, len);
        }
        static aug_result
        extract_(aug_mar* this_, const char* path) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->extractmar_(path);
        }
        static const void*
        getcontent_(aug_mar* this_, unsigned* size) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->getcontent_(*size);
        }
        static aug_rsize
        read_(aug_mar* this_, void* buf, unsigned len) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->readmar_(buf, len);
        }
        static unsigned
        getcontentsize_(aug_mar* this_) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->getcontentsize_();
        }
        static const aug_marvtbl*
        vtbl()
        {
            static const aug_marvtbl local = {
                cast_,
                retain_,
                release_,
                compact_,
                clear_,
                deln_,
                delp_,
                getn_,
                getp_,
                get_,
                putn_,
                putp_,
                put_,
                ntop_,
                pton_,
                getcount_,
                insert_,
                seek_,
                setcontent_,
                sync_,
                truncate_,
                write_,
                extract_,
                getcontent_,
                read_,
                getcontentsize_
            };
            return &local;
        }
    public:
        explicit
        mar(T* impl = 0)
        {
            this->mar_.vtbl_ = this->vtbl();
            this->mar_.impl_ = impl;
        }
        void
        reset(T* impl)
        {
            this->mar_.impl_ = impl;
        }
        aug_mar*
        get() AUG_NOTHROW
        {
            return &this->mar_;
        }
        operator aug::obref<aug_mar>() AUG_NOTHROW
        {
            return this->get();
        }
    };

    typedef aug::obref<aug_mar> marref;
    typedef aug::smartob<aug_mar> marptr;
}
#endif /* __cplusplus */

/**
 * @defgroup aug_marstore aug_marstore
 *
 * @ingroup Object
 *
 * @{
 */

struct aug_marstorevtbl {
    AUG_VTBL(aug_marstore);
    aug_result (*del_)(aug_marstore*, const char*);
    struct aug_mar_* (*get_)(aug_marstore*, const char*);
    aug_result (*put_)(aug_marstore*, const char*, struct aug_mar_*);
};

#define aug_delmar(this_, request) \
    (this_)->vtbl_->del_(this_, request)

#define aug_getmar(this_, request) \
    (this_)->vtbl_->get_(this_, request)

#define aug_putmar(this_, request, mar) \
    (this_)->vtbl_->put_(this_, request, mar)

/** @} */

#if defined(__cplusplus)
namespace aug {
    template <>
    struct object_traits<aug_marstore> {
        typedef aug_marstorevtbl vtbl;
        static const char*
        id() AUG_NOTHROW
        {
            return aug_marstoreid;
        }
    };
}

namespace aug {

    template <typename T>
    class marstore {

        aug_marstore marstore_;

        marstore(const marstore&);

        marstore&
        operator =(const marstore&);

        static void*
        cast_(aug_marstore* this_, const char* id) AUG_NOTHROW
        {
            return aug::retget(static_cast<T*>(this_->impl_)->cast_(id));
        }
        static void
        retain_(aug_marstore* this_) AUG_NOTHROW
        {
            static_cast<T*>(this_->impl_)->retain_();
        }
        static void
        release_(aug_marstore* this_) AUG_NOTHROW
        {
            static_cast<T*>(this_->impl_)->release_();
        }
        static aug_result
        del_(aug_marstore* this_, const char* request) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->delmar_(request);
        }
        static struct aug_mar_*
        get_(aug_marstore* this_, const char* request) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->getmar_(request);
        }
        static aug_result
        put_(aug_marstore* this_, const char* request, struct aug_mar_* mar) AUG_NOTHROW
        {
            return static_cast<T*>(this_->impl_)->putmar_(request, mar);
        }
        static const aug_marstorevtbl*
        vtbl()
        {
            static const aug_marstorevtbl local = {
                cast_,
                retain_,
                release_,
                del_,
                get_,
                put_
            };
            return &local;
        }
    public:
        explicit
        marstore(T* impl = 0)
        {
            this->marstore_.vtbl_ = this->vtbl();
            this->marstore_.impl_ = impl;
        }
        void
        reset(T* impl)
        {
            this->marstore_.impl_ = impl;
        }
        aug_marstore*
        get() AUG_NOTHROW
        {
            return &this->marstore_;
        }
        operator aug::obref<aug_marstore>() AUG_NOTHROW
        {
            return this->get();
        }
    };

    typedef aug::obref<aug_marstore> marstoreref;
    typedef aug::smartob<aug_marstore> marstoreptr;
}
#endif /* __cplusplus */

/* C++ definitions. */

#if defined(__cplusplus)
namespace aug {

    inline void
    compactmar(aug::obref<aug_mar> this_)
    {
        verify(this_.get()->vtbl_->compact_(this_.get()), this_);
    }

    inline unsigned
    clearfields(aug::obref<aug_mar> this_)
    {
        return static_cast<unsigned>(AUG_RESULT(verify(this_.get()->vtbl_->clear_(this_.get()), this_)));
    }

    inline void
    delfieldn(aug::obref<aug_mar> this_, unsigned n)
    {
        verify(this_.get()->vtbl_->deln_(this_.get(), n), this_);
    }

    inline unsigned
    delfieldp(aug::obref<aug_mar> this_, const char* name)
    {
        return static_cast<unsigned>(AUG_RESULT(verify(this_.get()->vtbl_->delp_(this_.get(), name), this_)));
    }

    inline unsigned
    getfieldn(aug::obref<aug_mar> this_, unsigned n, const void*& value)
    {
        return static_cast<unsigned>(AUG_RESULT(verify(this_.get()->vtbl_->getn_(this_.get(), n, &value), this_)));
    }

    inline unsigned
    getfieldp(aug::obref<aug_mar> this_, const char* name, const void*& value)
    {
        return static_cast<unsigned>(AUG_RESULT(verify(this_.get()->vtbl_->getp_(this_.get(), name, &value), this_)));
    }

    inline void
    getfield(aug::obref<aug_mar> this_, unsigned n, struct aug_field& field)
    {
        verify(this_.get()->vtbl_->get_(this_.get(), n, &field), this_);
    }

    inline void
    putfieldn(aug::obref<aug_mar> this_, unsigned n, const void* value, unsigned size)
    {
        verify(this_.get()->vtbl_->putn_(this_.get(), n, value, size), this_);
    }

    inline unsigned
    putfieldp(aug::obref<aug_mar> this_, const char* name, const void* value, unsigned size)
    {
        return static_cast<unsigned>(AUG_RESULT(verify(this_.get()->vtbl_->putp_(this_.get(), name, value, size), this_)));
    }

    inline unsigned
    putfield(aug::obref<aug_mar> this_, const struct aug_field& field)
    {
        return static_cast<unsigned>(AUG_RESULT(verify(this_.get()->vtbl_->put_(this_.get(), &field), this_)));
    }

    inline void
    fieldntop(aug::obref<aug_mar> this_, unsigned n, const char*& name)
    {
        verify(this_.get()->vtbl_->ntop_(this_.get(), n, &name), this_);
    }

    inline unsigned
    fieldpton(aug::obref<aug_mar> this_, const char* name)
    {
        return static_cast<unsigned>(AUG_RESULT(verify(this_.get()->vtbl_->pton_(this_.get(), name), this_)));
    }

    inline unsigned
    getfieldcount(aug::obref<aug_mar> this_) AUG_NOTHROW
    {
        return this_.get()->vtbl_->getcount_(this_.get());
    }

    inline void
    insertmar(aug::obref<aug_mar> this_, const char* path)
    {
        verify(this_.get()->vtbl_->insert_(this_.get(), path), this_);
    }

    inline size_t
    seekmar(aug::obref<aug_mar> this_, off_t offset, int whence)
    {
        return static_cast<size_t>(AUG_RESULT(verify(this_.get()->vtbl_->seek_(this_.get(), offset, whence), this_)));
    }

    inline void
    setcontent(aug::obref<aug_mar> this_, const void* cdata, unsigned size)
    {
        verify(this_.get()->vtbl_->setcontent_(this_.get(), cdata, size), this_);
    }

    inline void
    syncmar(aug::obref<aug_mar> this_)
    {
        verify(this_.get()->vtbl_->sync_(this_.get()), this_);
    }

    inline void
    truncatemar(aug::obref<aug_mar> this_, unsigned size)
    {
        verify(this_.get()->vtbl_->truncate_(this_.get(), size), this_);
    }

    inline size_t
    writemar(aug::obref<aug_mar> this_, const void* buf, unsigned len)
    {
        return static_cast<size_t>(AUG_RESULT(verify(this_.get()->vtbl_->write_(this_.get(), buf, len), this_)));
    }

    inline void
    extractmar(aug::obref<aug_mar> this_, const char* path)
    {
        verify(this_.get()->vtbl_->extract_(this_.get(), path), this_);
    }

    inline const void*
    getcontent(aug::obref<aug_mar> this_, unsigned& size) AUG_NOTHROW
    {
        return this_.get()->vtbl_->getcontent_(this_.get(), &size);
    }

    inline size_t
    readmar(aug::obref<aug_mar> this_, void* buf, unsigned len)
    {
        return static_cast<size_t>(AUG_RESULT(verify(this_.get()->vtbl_->read_(this_.get(), buf, len), this_)));
    }

    inline unsigned
    getcontentsize(aug::obref<aug_mar> this_) AUG_NOTHROW
    {
        return this_.get()->vtbl_->getcontentsize_(this_.get());
    }

    template <typename T>
    class mar_base {
        mar<T> mar_;
        int refs_;
    protected:
        ~mar_base()
        {
            /* Always deleted via derived, so no need to be virtual. */
        }
        mar_base()
            : refs_(1)
        {
            this->mar_.reset(static_cast<T*>(this));
        }
    public:
        aug::smartob<aug_object>
        cast_(const char* id) AUG_NOTHROW
        {
            if (aug::equalid<aug_object>(id) || aug::equalid<aug_mar>(id))
                return aug::object_retain<aug_object>(this->mar_);
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
                delete static_cast<T*>(this);
        }
        static aug::smartob<aug_mar>
        attach(T* ptr)
        {
            return aug::object_attach<aug_mar>(ptr->mar_);
        }
    };

    template <typename T>
    class scoped_mar_base {
        mar<T> mar_;
    protected:
        ~scoped_mar_base()
        {
        }
        scoped_mar_base()
        {
            this->mar_.reset(static_cast<T*>(this));
        }
    public:
        aug::smartob<aug_object>
        cast_(const char* id) AUG_NOTHROW
        {
            if (aug::equalid<aug_object>(id) || aug::equalid<aug_mar>(id))
                return aug::object_retain<aug_object>(this->mar_);
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
        aug_object*
        base() AUG_NOTHROW
        {
            return aug::obref<aug_object>(this->mar_).get();
        }
        aug_mar*
        get() AUG_NOTHROW
        {
            return this->mar_.get();
        }
        operator aug::obref<aug_mar>() AUG_NOTHROW
        {
            return this->mar_;
        }
    };

    template <typename T>
    class mar_wrapper {
        mar<mar_wrapper<T> > mar_;
        T impl_;
        int refs_;
        explicit
        mar_wrapper(const T& impl)
            : impl_(impl),
              refs_(1)
        {
            this->mar_.reset(this);
        }
    public:
        aug::smartob<aug_object>
        cast_(const char* id) AUG_NOTHROW
        {
            if (aug::equalid<aug_object>(id) || aug::equalid<aug_mar>(id))
                return aug::object_retain<aug_object>(this->mar_);
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
        aug_result
        compactmar_() AUG_NOTHROW
        {
            return this->impl_.compactmar_();
        }
        aug_rint
        clearfields_() AUG_NOTHROW
        {
            return this->impl_.clearfields_();
        }
        aug_result
        delfieldn_(unsigned n) AUG_NOTHROW
        {
            return this->impl_.delfieldn_(n);
        }
        aug_rint
        delfieldp_(const char* name) AUG_NOTHROW
        {
            return this->impl_.delfieldp_(name);
        }
        aug_rint
        getfieldn_(unsigned n, const void*& value) AUG_NOTHROW
        {
            return this->impl_.getfieldn_(n, value);
        }
        aug_rint
        getfieldp_(const char* name, const void*& value) AUG_NOTHROW
        {
            return this->impl_.getfieldp_(name, value);
        }
        aug_result
        getfield_(unsigned n, struct aug_field& field) AUG_NOTHROW
        {
            return this->impl_.getfield_(n, field);
        }
        aug_result
        putfieldn_(unsigned n, const void* value, unsigned size) AUG_NOTHROW
        {
            return this->impl_.putfieldn_(n, value, size);
        }
        aug_rint
        putfieldp_(const char* name, const void* value, unsigned size) AUG_NOTHROW
        {
            return this->impl_.putfieldp_(name, value, size);
        }
        aug_rint
        putfield_(const struct aug_field& field) AUG_NOTHROW
        {
            return this->impl_.putfield_(field);
        }
        aug_result
        fieldntop_(unsigned n, const char*& name) AUG_NOTHROW
        {
            return this->impl_.fieldntop_(n, name);
        }
        aug_rint
        fieldpton_(const char* name) AUG_NOTHROW
        {
            return this->impl_.fieldpton_(name);
        }
        unsigned
        getfieldcount_() AUG_NOTHROW
        {
            return this->impl_.getfieldcount_();
        }
        aug_result
        insertmar_(const char* path) AUG_NOTHROW
        {
            return this->impl_.insertmar_(path);
        }
        aug_rsize
        seekmar_(off_t offset, int whence) AUG_NOTHROW
        {
            return this->impl_.seekmar_(offset, whence);
        }
        aug_result
        setcontent_(const void* cdata, unsigned size) AUG_NOTHROW
        {
            return this->impl_.setcontent_(cdata, size);
        }
        aug_result
        syncmar_() AUG_NOTHROW
        {
            return this->impl_.syncmar_();
        }
        aug_result
        truncatemar_(unsigned size) AUG_NOTHROW
        {
            return this->impl_.truncatemar_(size);
        }
        aug_rsize
        writemar_(const void* buf, unsigned len) AUG_NOTHROW
        {
            return this->impl_.writemar_(buf, len);
        }
        aug_result
        extractmar_(const char* path) AUG_NOTHROW
        {
            return this->impl_.extractmar_(path);
        }
        const void*
        getcontent_(unsigned& size) AUG_NOTHROW
        {
            return this->impl_.getcontent_(size);
        }
        aug_rsize
        readmar_(void* buf, unsigned len) AUG_NOTHROW
        {
            return this->impl_.readmar_(buf, len);
        }
        unsigned
        getcontentsize_() AUG_NOTHROW
        {
            return this->impl_.getcontentsize_();
        }
        static aug::smartob<aug_mar>
        create(const T& impl = T())
        {
            mar_wrapper* ptr(new mar_wrapper(impl));
            return aug::object_attach<aug_mar>(ptr->mar_);
        }
    };

    template <typename T>
    class scoped_mar_wrapper {
        mar<scoped_mar_wrapper<T> > mar_;
        T impl_;
    public:
        explicit
        scoped_mar_wrapper(const T& impl = T())
            : impl_(impl)
        {
            this->mar_.reset(this);
        }
        aug::smartob<aug_object>
        cast_(const char* id) AUG_NOTHROW
        {
            if (aug::equalid<aug_object>(id) || aug::equalid<aug_mar>(id))
                return aug::object_retain<aug_object>(this->mar_);
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
        aug_result
        compactmar_() AUG_NOTHROW
        {
            return this->impl_.compactmar_();
        }
        aug_rint
        clearfields_() AUG_NOTHROW
        {
            return this->impl_.clearfields_();
        }
        aug_result
        delfieldn_(unsigned n) AUG_NOTHROW
        {
            return this->impl_.delfieldn_(n);
        }
        aug_rint
        delfieldp_(const char* name) AUG_NOTHROW
        {
            return this->impl_.delfieldp_(name);
        }
        aug_rint
        getfieldn_(unsigned n, const void*& value) AUG_NOTHROW
        {
            return this->impl_.getfieldn_(n, value);
        }
        aug_rint
        getfieldp_(const char* name, const void*& value) AUG_NOTHROW
        {
            return this->impl_.getfieldp_(name, value);
        }
        aug_result
        getfield_(unsigned n, struct aug_field& field) AUG_NOTHROW
        {
            return this->impl_.getfield_(n, field);
        }
        aug_result
        putfieldn_(unsigned n, const void* value, unsigned size) AUG_NOTHROW
        {
            return this->impl_.putfieldn_(n, value, size);
        }
        aug_rint
        putfieldp_(const char* name, const void* value, unsigned size) AUG_NOTHROW
        {
            return this->impl_.putfieldp_(name, value, size);
        }
        aug_rint
        putfield_(const struct aug_field& field) AUG_NOTHROW
        {
            return this->impl_.putfield_(field);
        }
        aug_result
        fieldntop_(unsigned n, const char*& name) AUG_NOTHROW
        {
            return this->impl_.fieldntop_(n, name);
        }
        aug_rint
        fieldpton_(const char* name) AUG_NOTHROW
        {
            return this->impl_.fieldpton_(name);
        }
        unsigned
        getfieldcount_() AUG_NOTHROW
        {
            return this->impl_.getfieldcount_();
        }
        aug_result
        insertmar_(const char* path) AUG_NOTHROW
        {
            return this->impl_.insertmar_(path);
        }
        aug_rsize
        seekmar_(off_t offset, int whence) AUG_NOTHROW
        {
            return this->impl_.seekmar_(offset, whence);
        }
        aug_result
        setcontent_(const void* cdata, unsigned size) AUG_NOTHROW
        {
            return this->impl_.setcontent_(cdata, size);
        }
        aug_result
        syncmar_() AUG_NOTHROW
        {
            return this->impl_.syncmar_();
        }
        aug_result
        truncatemar_(unsigned size) AUG_NOTHROW
        {
            return this->impl_.truncatemar_(size);
        }
        aug_rsize
        writemar_(const void* buf, unsigned len) AUG_NOTHROW
        {
            return this->impl_.writemar_(buf, len);
        }
        aug_result
        extractmar_(const char* path) AUG_NOTHROW
        {
            return this->impl_.extractmar_(path);
        }
        const void*
        getcontent_(unsigned& size) AUG_NOTHROW
        {
            return this->impl_.getcontent_(size);
        }
        aug_rsize
        readmar_(void* buf, unsigned len) AUG_NOTHROW
        {
            return this->impl_.readmar_(buf, len);
        }
        unsigned
        getcontentsize_() AUG_NOTHROW
        {
            return this->impl_.getcontentsize_();
        }
        aug_object*
        base() AUG_NOTHROW
        {
            return aug::obref<aug_object>(this->mar_).get();
        }
        aug_mar*
        get() AUG_NOTHROW
        {
            return this->mar_.get();
        }
        operator aug::obref<aug_mar>() AUG_NOTHROW
        {
            return this->mar_;
        }
    };

    typedef aug::smartob<aug_mar> marptr;

    inline void
    delmar(aug::obref<aug_marstore> this_, const char* request)
    {
        verify(this_.get()->vtbl_->del_(this_.get(), request), this_);
    }

    inline struct aug_mar_*
    getmar(aug::obref<aug_marstore> this_, const char* request)
    {
        return verify(this_.get()->vtbl_->get_(this_.get(), request), this_);
    }

    inline void
    putmar(aug::obref<aug_marstore> this_, const char* request, struct aug_mar_* mar)
    {
        verify(this_.get()->vtbl_->put_(this_.get(), request, mar), this_);
    }

    template <typename T>
    class marstore_base {
        marstore<T> marstore_;
        int refs_;
    protected:
        ~marstore_base()
        {
            /* Always deleted via derived, so no need to be virtual. */
        }
        marstore_base()
            : refs_(1)
        {
            this->marstore_.reset(static_cast<T*>(this));
        }
    public:
        aug::smartob<aug_object>
        cast_(const char* id) AUG_NOTHROW
        {
            if (aug::equalid<aug_object>(id) || aug::equalid<aug_marstore>(id))
                return aug::object_retain<aug_object>(this->marstore_);
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
                delete static_cast<T*>(this);
        }
        static aug::smartob<aug_marstore>
        attach(T* ptr)
        {
            return aug::object_attach<aug_marstore>(ptr->marstore_);
        }
    };

    template <typename T>
    class scoped_marstore_base {
        marstore<T> marstore_;
    protected:
        ~scoped_marstore_base()
        {
        }
        scoped_marstore_base()
        {
            this->marstore_.reset(static_cast<T*>(this));
        }
    public:
        aug::smartob<aug_object>
        cast_(const char* id) AUG_NOTHROW
        {
            if (aug::equalid<aug_object>(id) || aug::equalid<aug_marstore>(id))
                return aug::object_retain<aug_object>(this->marstore_);
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
        aug_object*
        base() AUG_NOTHROW
        {
            return aug::obref<aug_object>(this->marstore_).get();
        }
        aug_marstore*
        get() AUG_NOTHROW
        {
            return this->marstore_.get();
        }
        operator aug::obref<aug_marstore>() AUG_NOTHROW
        {
            return this->marstore_;
        }
    };

    template <typename T>
    class marstore_wrapper {
        marstore<marstore_wrapper<T> > marstore_;
        T impl_;
        int refs_;
        explicit
        marstore_wrapper(const T& impl)
            : impl_(impl),
              refs_(1)
        {
            this->marstore_.reset(this);
        }
    public:
        aug::smartob<aug_object>
        cast_(const char* id) AUG_NOTHROW
        {
            if (aug::equalid<aug_object>(id) || aug::equalid<aug_marstore>(id))
                return aug::object_retain<aug_object>(this->marstore_);
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
        aug_result
        delmar_(const char* request) AUG_NOTHROW
        {
            return this->impl_.delmar_(request);
        }
        struct aug_mar_*
        getmar_(const char* request) AUG_NOTHROW
        {
            return this->impl_.getmar_(request);
        }
        aug_result
        putmar_(const char* request, struct aug_mar_* mar) AUG_NOTHROW
        {
            return this->impl_.putmar_(request, mar);
        }
        static aug::smartob<aug_marstore>
        create(const T& impl = T())
        {
            marstore_wrapper* ptr(new marstore_wrapper(impl));
            return aug::object_attach<aug_marstore>(ptr->marstore_);
        }
    };

    template <typename T>
    class scoped_marstore_wrapper {
        marstore<scoped_marstore_wrapper<T> > marstore_;
        T impl_;
    public:
        explicit
        scoped_marstore_wrapper(const T& impl = T())
            : impl_(impl)
        {
            this->marstore_.reset(this);
        }
        aug::smartob<aug_object>
        cast_(const char* id) AUG_NOTHROW
        {
            if (aug::equalid<aug_object>(id) || aug::equalid<aug_marstore>(id))
                return aug::object_retain<aug_object>(this->marstore_);
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
        aug_result
        delmar_(const char* request) AUG_NOTHROW
        {
            return this->impl_.delmar_(request);
        }
        struct aug_mar_*
        getmar_(const char* request) AUG_NOTHROW
        {
            return this->impl_.getmar_(request);
        }
        aug_result
        putmar_(const char* request, struct aug_mar_* mar) AUG_NOTHROW
        {
            return this->impl_.putmar_(request, mar);
        }
        aug_object*
        base() AUG_NOTHROW
        {
            return aug::obref<aug_object>(this->marstore_).get();
        }
        aug_marstore*
        get() AUG_NOTHROW
        {
            return this->marstore_.get();
        }
        operator aug::obref<aug_marstore>() AUG_NOTHROW
        {
            return this->marstore_;
        }
    };

    typedef aug::smartob<aug_marstore> marstoreptr;
}
#endif /* __cplusplus */

#endif /* AUGEXT_MAR_H */
