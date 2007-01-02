/* Copyright (c) 2004-2006, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#define AUGNET_BUILD
#include "augnet/conn.h"

static const char rcsid[] = "$Id$";

#include "augutil/var.h"

#include "augsys/errinfo.h"
#include "augsys/errno.h"
#include "augsys/lock.h"

#include <stdlib.h>

struct aug_conn_ {
    AUG_ENTRY(aug_conn_);
    int fd_;
    aug_conncb_t cb_;
    struct aug_var arg_;
};

static struct aug_conns free_ = AUG_HEAD_INITIALIZER(free_);
AUG_ALLOCATOR(allocate_, &free_, aug_conn_, 64)

AUGNET_API int
aug_freeconns(struct aug_conns* conns)
{
    struct aug_conn_* it;
    AUG_FOREACH(it, conns)
        aug_freevar(&it->arg_);

    if (!AUG_EMPTY(conns)) {

        aug_lock();
        AUG_CONCAT(&free_, conns);
        aug_unlock();
    }
    return 0;
}

AUGNET_API int
aug_insertconn(struct aug_conns* conns, int fd, aug_conncb_t cb,
               const struct aug_var* arg)
{
    struct aug_conn_* conn;

    aug_lock();
    if (!(conn = allocate_())) {
        aug_unlock();
        return -1;
    }
    aug_unlock();

    conn->fd_ = fd;
    conn->cb_ = cb;
    aug_setvar(&conn->arg_, arg);

    AUG_INSERT_TAIL(conns, conn);
    return 0;
}

AUGNET_API int
aug_removeconn(struct aug_conns* conns, int fd)
{
    struct aug_conn_* it;

    AUG_FOREACH(it, conns)
        if (it->fd_ == fd)
            break;

    if (!it) {
        aug_seterrinfo(NULL, __FILE__, __LINE__, AUG_SRCLOCAL, AUG_EEXIST,
                       AUG_MSG("no connection for descriptor '%d'"), (int)fd);
        return -1;
    }

    AUG_REMOVE(conns, it, aug_conn_);

    aug_freevar(&it->arg_);
    aug_lock();
    AUG_INSERT_TAIL(&free_, it);
    aug_unlock();

    return 0;
}

AUGNET_API int
aug_processconns(struct aug_conns* conns)
{
    struct aug_conn_* it, ** prev;
    struct aug_conns tail;
    AUG_INIT(&tail);

    prev = &AUG_FIRST(conns);
    while ((it = *prev)) {

        if (!(it->cb_(it->fd_, &it->arg_, &tail))) {

            AUG_REMOVE_PREVPTR(it, prev, conns);

            aug_freevar(&it->arg_);
            aug_lock();
            AUG_INSERT_TAIL(&free_, it);
            aug_unlock();

        } else
            prev = &AUG_NEXT(it);
    }

    AUG_CONCAT(conns, &tail);
    return 0;
}
