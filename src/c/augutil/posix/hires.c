/* Copyright (c) 2004-2007, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#include "augsys/time.h"

#include "augctx/base.h"
#include "augctx/errinfo.h"

#include <errno.h>  /* ENOMEM */
#include <stdlib.h> /* malloc() */

struct aug_hires_ {
    aug_mpool* mpool_;
    aug_clock* clock_;
    struct timeval start_;
};

AUGUTIL_API aug_hires_t
aug_createhires(aug_mpool* mpool)
{
    aug_clock* clock;
    aug_hires_t hires;

    if (!(clock = aug_createclock(mpool, 0)))
        return NULL;

    if (!(hires = aug_allocmem(mpool, sizeof(struct aug_hires_))))
        goto fail1;

    hires->mpool_ = mpool;
    hires->clock_ = clock;

    if (aug_gettimeofday(clock, &hires->start_) < 0)
        goto fail2;

    /* Success. */

    aug_retain(mpool);
    return hires;

 fail2:
    aug_freemem(mpool, hires);
 fail1:
    aug_release(clock);
    return NULL;
}

AUGUTIL_API aug_result
aug_destroyhires(aug_hires_t hires)
{
    aug_mpool* mpool = hires->mpool_;
    aug_clock* clock = hires->clock_;
    aug_freemem(mpool, hires);
    aug_release(clock);
    aug_release(mpool);
    return AUG_SUCCESS;
}

AUGUTIL_API aug_result
aug_resethires(aug_hires_t hires)
{
    return aug_gettimeofday(hires->clock_, &hires->start_);
}

AUGUTIL_API double*
aug_elapsed(aug_hires_t hires, double* sec)
{
    struct timeval now;
    if (aug_gettimeofday(hires->clock_, &now) < 0)
        return NULL;
    aug_tvsub(&now, &hires->start_);
    *sec = (double)now.tv_sec + ((double)now.tv_usec / 1000000.0);
    return sec;
}