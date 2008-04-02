/* Copyright (c) 2004-2007, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#define AUGUTIL_BUILD
#include "augutil/conv.h"
#include "augctx/defs.h"

AUG_RCSID("$Id$");

#include "augctx/errinfo.h"

#include <errno.h>
#include <limits.h> /* UINT_MAX */
#include <stdlib.h> /* strtoul() */

AUGUTIL_API unsigned long*
aug_strtoul(aug_ctx* ctx, unsigned long* dst, const char* src, int base)
{
    char* end;
    unsigned long ul;

    errno = 0;
    ul = strtoul(src, &end, base);

    if (0 != errno) {

        aug_setposixerrinfo(aug_geterrinfo(ctx), __FILE__, __LINE__, errno);
        dst = NULL;

    } else if ('\0' != *end) {

        /* The string was only partially processed. */

        aug_seterrinfo(aug_geterrinfo(ctx), __FILE__, __LINE__, "aug",
                       AUG_EPARSE, AUG_MSG("partial conversion"));
        dst = NULL;

    } else {

        /* Success. */

        *dst = ul;
    }
    return dst;
}

AUGUTIL_API unsigned*
aug_strtoui(aug_ctx* ctx, unsigned* dst, const char* src, int base)
{
    unsigned long ul;
    if (!aug_strtoul(ctx, &ul, src, base))
        return NULL;

    if (UINT_MAX < ul) {

        /* Bounds exceeded for target type. */

        aug_seterrinfo(aug_geterrinfo(ctx), __FILE__, __LINE__, "aug",
                       AUG_ELIMIT, AUG_MSG("max integer value exceeded"));
        return NULL;
    }

    *dst = (unsigned)ul;
    return dst;
}

AUGUTIL_API unsigned short*
aug_strtous(aug_ctx* ctx, unsigned short* dst, const char* src, int base)
{
    unsigned long ul;
    if (!aug_strtoul(ctx, &ul, src, base))
        return NULL;

    if (USHRT_MAX < ul) {

        /* Bounds exceeded for target type. */

        aug_seterrinfo(aug_geterrinfo(ctx), __FILE__, __LINE__, "aug",
                       AUG_ELIMIT, AUG_MSG("max integer value exceeded"));
        return NULL;
    }

    *dst = (unsigned short)ul;
    return dst;
}
