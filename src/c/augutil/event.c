/* Copyright (c) 2004-2007, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#define AUGUTIL_BUILD
#include "augutil/event.h"
#include "augsys/defs.h"

AUG_RCSID("$Id$");

#include "augutil/object.h"

#include "augsys/barrier.h"
#include "augsys/errinfo.h"
#include "augsys/errno.h"
#include "augsys/unistd.h"

#include <errno.h>
#include <signal.h>

#if defined(_WIN32)
# define SIGHUP  1
# define SIGUSR1 10
#endif /* _WIN32 */

static int
readall_(int fd, char* buf, size_t n)
{
    /* Ensure all bytes are read and ignore any interrupts. */

    while (0 != n) {

        int ret = aug_read(fd, buf, n);
        if (-1 == ret) {
            if (EINTR == aug_errno())
                continue;

            return -1;
        }
        buf += ret, n -= ret;
    }
    return 0;
}

static int
writeall_(int fd, const char* buf, size_t n)
{
    /* Ensure all bytes are written and ignore any interrupts. */

    while (0 != n) {

        int ret = aug_write(fd, buf, n);
        if (-1 == ret) {
            if (EINTR == aug_errno())
                continue;

            return -1;
        }
        buf += ret, n -= ret;
    }
    return 0;
}

AUGUTIL_API struct aug_event*
aug_setsigevent(struct aug_event* event, int sig)
{
    switch (sig) {
    case SIGHUP:
        event->type_ = AUG_EVENTRECONF;
        break;
    case SIGUSR1:
        event->type_ = AUG_EVENTSTATUS;
        break;
    case SIGINT:
    case SIGTERM:
        event->type_ = AUG_EVENTSTOP;
        break;
    default:
        event->type_ = AUG_EVENTSIGNAL;
    }
    event->ob_ = (aug_object*)aug_createlongob(sig, NULL);
    return event;
}

AUGUTIL_API struct aug_event*
aug_readevent(int fd, struct aug_event* event)
{
    if (-1 == readall_(fd, (char*)event, sizeof(*event)))
        return NULL;

    /* Ensure writes are visible: ensure that any components of the event
       object are read from main memory. */

    AUG_RMB();

    return event;
}

AUGUTIL_API const struct aug_event*
aug_writeevent(int fd, const struct aug_event* event)
{
    /* Flush pending writes to main memory: ensure that the event object is
       visible to other threads. */

    AUG_WMB();

    /* Must increment before write. */

    if (event->ob_)
        aug_incref(event->ob_);

    if (-1 == writeall_(fd, (const char*)event, sizeof(*event))) {
        if (event->ob_)
            aug_decref(event->ob_);
        return NULL;
    }

    return event;
}
