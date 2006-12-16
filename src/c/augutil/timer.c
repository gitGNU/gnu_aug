/* Copyright (c) 2004-2006, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#define AUGUTIL_BUILD
#include "augutil/timer.h"

static const char rcsid[] = "$Id$";

#include "augsys/errinfo.h"
#include "augsys/errno.h"
#include "augsys/lock.h"
#include "augsys/time.h"
#include "augsys/base.h"

#include <stdlib.h>

struct aug_timer_ {
    AUG_ENTRY(aug_timer_);
    int id_;
    unsigned ms_;
    struct timeval tv_;
    aug_timercb_t cb_;
    struct aug_var arg_;
};

static struct aug_timers free_ = AUG_HEAD_INITIALIZER(free_);
AUG_ALLOCATOR(allocate_, &free_, aug_timer_, 64)

static int
expiry_(struct timeval* tv, unsigned ms)
{
    struct timeval local;
    if (-1 == aug_gettimeofday(tv, NULL))
        return -1;

    aug_tvadd(tv, aug_mstotv(&local, ms));
    return 0;
}

static void
insert_(struct aug_timers* timers, struct aug_timer_* timer)
{
    struct aug_timer_* it, * prev;

    it = AUG_FIRST(timers);

    if (!it || timercmp(&timer->tv_, &it->tv_, <)) {

        AUG_INSERT_HEAD(timers, timer);

    } else {

        for (prev = it; (it = AUG_NEXT(prev)); prev = it) {

            if (timercmp(&timer->tv_, &it->tv_, <))
                break;
        }
        AUG_INSERT_AFTER(timers, prev, timer);
    }
}

AUGUTIL_API int
aug_freetimers(struct aug_timers* timers)
{
    struct aug_timer_* it;
    AUG_FOREACH(it, timers)
        aug_freevar(&it->arg_);

    if (!AUG_EMPTY(timers)) {

        aug_lock();
        AUG_CONCAT(&free_, timers);
        aug_unlock();
    }
    return 0;
}

AUGUTIL_API int
aug_settimer(struct aug_timers* timers, int id, unsigned ms,
             aug_timercb_t cb, const struct aug_var* arg)
{
    struct timeval tv;
    struct aug_timer_* timer;

    if (id <= 0)
        id = aug_nextid();
    else
        aug_canceltimer(timers, id);

    if (-1 == expiry_(&tv, ms)) {
        aug_freevar(arg);
        return -1;
    }

    aug_lock();
    if (!(timer = allocate_())) {
        aug_unlock();
        aug_freevar(arg);
        return -1;
    }
    aug_unlock();

    timer->id_ = id;
    timer->ms_ = ms;
    timer->tv_.tv_sec = tv.tv_sec;
    timer->tv_.tv_usec = tv.tv_usec;
    timer->cb_ = cb;
    aug_setvar(&timer->arg_, arg);
    insert_(timers, timer);

    return timer->id_;
}

AUGUTIL_API int
aug_resettimer(struct aug_timers* timers, int id, unsigned ms)
{
    struct aug_timer_* it, ** prev;

    prev = &AUG_FIRST(timers);
    while ((it = *prev)) {

        if (it->id_ == id) {

            AUG_REMOVE_PREVPTR(it, prev, timers);
            if (ms) /* May be zero. */
                it->ms_ = ms;

            if (-1 == expiry_(&it->tv_, it->ms_)) {

                aug_freevar(&it->arg_);
                aug_lock();
                AUG_INSERT_TAIL(&free_, it);
                aug_unlock();
                return -1;
            }

            insert_(timers, it);
            return 0;

        } else
            prev = &AUG_NEXT(it);
    }
    return AUG_RETNONE;
}

AUGUTIL_API int
aug_canceltimer(struct aug_timers* timers, int id)
{
    struct aug_timer_* it, ** prev;

    prev = &AUG_FIRST(timers);
    while ((it = *prev)) {

        if (it->id_ == id) {

            AUG_REMOVE_PREVPTR(it, prev, timers);

            aug_freevar(&it->arg_);
            aug_lock();
            AUG_INSERT_TAIL(&free_, it);
            aug_unlock();
            return 0;

        } else
            prev = &AUG_NEXT(it);
    }
    return AUG_RETNONE;
}

AUGUTIL_API int
aug_expired(struct aug_timers* timers, int id)
{
    struct aug_timer_* it;
    AUG_FOREACH(it, timers)
        if (it->id_ == id)
            return 0;

    return 1;
}

AUGUTIL_API int
aug_processtimers(struct aug_timers* timers, int force, struct timeval* next)
{
    struct timeval now;
    struct aug_timer_* it;

    if ((it = AUG_FIRST(timers))) {

        if (-1 == aug_gettimeofday(&now, NULL))
            return -1;

        /* Force, at least, the first timer to expire. */

        if (force) {
            it->tv_.tv_sec = now.tv_sec;
            it->tv_.tv_usec = now.tv_usec;
        }

        do {

            if (timercmp(&now, &it->tv_, <))
                break;

            /* Remove first to avoid another being added in front of this
               one. */

            AUG_REMOVE_HEAD(timers);

            (*it->cb_)(it->id_, &it->arg_, &it->ms_, timers);
            if (it->ms_) {

                if (-1 == expiry_(&it->tv_, it->ms_))
                    aug_perrinfo(NULL, "expiry_() failed");
                else
                    insert_(timers, it);

            } else {

                /* A zero ms value cancels the timer. */

                aug_freevar(&it->arg_);
                aug_lock();
                AUG_INSERT_TAIL(&free_, it);
                aug_unlock();
            }

        } while ((it = AUG_FIRST(timers)));
    }

    if (next) {

        if (!it)
            next->tv_sec = next->tv_usec = 0; /* Forever: no timeout. */
        else {

            next->tv_sec = it->tv_.tv_sec;
            next->tv_usec = it->tv_.tv_usec;
            aug_tvsub(next, &now);
        }
    }
    return 0;
}
