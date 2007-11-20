/* Copyright (c) 2004-2007, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#ifndef AUGOBJ_H
#define AUGOBJ_H

#include <stddef.h>
#include <sys/types.h> /* size_t */

/*
  The offsetof() macro can only be used for PODs.  GCC will emit the following
  warning when used on non-POD types:

  warning: (perhaps the `offsetof' macro was used incorrectly)
 */

#if !defined(offsetof)
# define offsetof(s, m) (size_t)&(((s*)0)->m)
#endif /* !offsetof */

#define AUG_PODIMPL(s, m, ptr)                          \
    (ptr ? (s*)((char*)(ptr) - offsetof(s, m)) : NULL)

#if !defined(AUG_MKSTR)
# define AUG_MKSTR_(x) #x
# define AUG_MKSTR(x) AUG_MKSTR_(x)
#endif /* !AUG_MKSTR */

#define AUG_OBJECTDECL(type)                        \
    struct type##vtbl;                              \
    typedef struct {                                \
            const struct type##vtbl* vtbl_;         \
            void* impl_;                            \
    } type;                                         \
    static const char type##id[] = AUG_MKSTR(type)

#define AUG_OBJECT(type)                        \
    void* (*cast_)(type*, const char*);         \
    int (*retain_)(type*);                      \
    int (*release_)(type*)

AUG_OBJECTDECL(aug_object);

struct aug_objectvtbl {
    AUG_OBJECT(aug_object);
};

#define aug_castobject(obj, type)                                   \
    ((aug_object*)(obj))->vtbl_->cast_((aug_object*)(obj), type)

#define aug_retainobject(obj)                                   \
    ((aug_object*)(obj))->vtbl_->retain_((aug_object*)(obj))

#define aug_releaseobject(obj)                                  \
    ((aug_object*)(obj))->vtbl_->release_((aug_object*)(obj))

AUG_OBJECTDECL(aug_blob);

struct aug_blobvtbl {
    AUG_OBJECT(aug_blob);
    const void* (*data_)(aug_blob*, size_t*);
};

#define aug_blobdata(obj, size)                 \
    (obj)->vtbl_->data_(obj, size)

#endif /* AUGOBJ_H */
