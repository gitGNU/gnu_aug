/* Copyright (c) 2004-2007, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#include "augsys/base.h"
#include "augsys/errinfo.h"
#include "augsys/errno.h"
#include "augsys/socket.h"
#include "augsys/time.h" /* aug_tvtoms() */
#include "augsys/unistd.h"

#include <winsock2.h>    /* select() */

#include <stdlib.h>      /* malloc() */

struct set_ {
    fd_set rd_, wr_, ex_;
};

struct aug_mplexer_ {
    struct set_ in_, out_;
    size_t bits_;
};

static void
zeroset_(struct set_* p)
{
    FD_ZERO(&p->rd_);
    FD_ZERO(&p->wr_);
    FD_ZERO(&p->ex_);
}

static unsigned short
external_(struct set_* p, SOCKET h)
{
    unsigned short dst = 0;

    if (FD_ISSET(h, &p->rd_))
        dst |= AUG_IOEVENTRD;

    if (FD_ISSET(h, &p->wr_))
        dst |= AUG_IOEVENTWR;

    if (FD_ISSET(h, &p->ex_))
        dst |= AUG_IOEVENTEX;

    return dst;
}

static int
setioevents_(struct set_* p, SOCKET h, unsigned short mask)
{
    unsigned short cur = external_(p, h);
    unsigned short set = ~cur & mask;
    unsigned short unset = cur & ~mask;
    int bits = 0;

    if (set & AUG_IOEVENTRD) {

        FD_SET(h, &p->rd_);
        ++bits;

    } else if (unset & AUG_IOEVENTRD) {

        FD_CLR(h, &p->rd_);
        --bits;
    }

    if (set & AUG_IOEVENTWR) {

        FD_SET(h, &p->wr_);
        ++bits;

    } else if (unset & AUG_IOEVENTWR) {

        FD_CLR(h, &p->wr_);
        --bits;
    }

    if (set & AUG_IOEVENTEX) {

        FD_SET(h, &p->ex_);
        ++bits;

    } else if (unset & AUG_IOEVENTEX) {

        FD_CLR(h, &p->ex_);
        --bits;
    }

    return bits;
}

AUGSYS_API aug_mplexer_t
aug_createmplexer(void)
{
    aug_mplexer_t mplexer = malloc(sizeof(struct aug_mplexer_));
    if (!mplexer) {
        aug_setposixerrinfo(NULL, __FILE__, __LINE__, ENOMEM);
        return NULL;
    }

    zeroset_(&mplexer->in_);
    zeroset_(&mplexer->out_);
    mplexer->bits_ = 0;
    return mplexer;
}

AUGSYS_API int
aug_destroymplexer(aug_mplexer_t mplexer)
{
    free(mplexer);
    return 0;
}

AUGSYS_API int
aug_setioeventmask(aug_mplexer_t mplexer, int fd, unsigned short mask)
{
    /* Although this condition does not necessarily mean that the set has been
       exhausted, it gives a reasonable approximation given the costly
       alternatives. */

    if (FD_SETSIZE <= fd) {
        aug_setwin32errinfo(NULL, __FILE__, __LINE__, WSAEMFILE);
        return -1;
    }

    if (mask & ~AUG_IOEVENTALL) {
        aug_seterrinfo(NULL, __FILE__, __LINE__, AUG_SRCLOCAL, AUG_EINVAL,
                       AUG_MSG("invalid ioevent mask"));
        return -1;
    }

    mplexer->bits_ += setioevents_(&mplexer->in_, _get_osfhandle(fd), mask);
    return 0;
}

AUGSYS_API int
aug_waitioevents(aug_mplexer_t mplexer, const struct timeval* timeout)
{
    int ret;
    mplexer->out_ = mplexer->in_;

    if (0 == mplexer->bits_) {
        Sleep(timeout ? aug_tvtoms(timeout) : INFINITE);
        return 0;
    }

    /* Note: WinSock ignores the nfds argument. */

    if (SOCKET_ERROR ==
        (ret = select(-1, &mplexer->out_.rd_, &mplexer->out_.wr_,
                      &mplexer->out_.ex_, timeout))) {

        if (WSAEINTR == aug_setwin32errinfo(NULL, __FILE__, __LINE__,
                                            WSAGetLastError()))
            ret = AUG_RETINTR;
    }
    return ret;
}

AUGSYS_API int
aug_ioeventmask(aug_mplexer_t mplexer, int fd)
{
    return external_(&mplexer->in_, _get_osfhandle(fd));
}

AUGSYS_API int
aug_ioevents(aug_mplexer_t mplexer, int fd)
{
    return external_(&mplexer->out_, _get_osfhandle(fd));
}

AUGSYS_API int
aug_mplexerpipe(int fds[2])
{
    int local[2];

    if (-1 == aug_socketpair(AF_UNIX, SOCK_STREAM, 0, local))
        return -1;

    fds[0] = local[0];
    fds[1] = local[1];
    return 0;
}
