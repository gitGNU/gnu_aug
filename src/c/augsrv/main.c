/* Copyright (c) 2004-2007, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#define AUGSRV_BUILD
#include "augsrv/main.h"
#include "augctx/defs.h"

AUG_RCSID("$Id$");

#include "augsrv/base.h"
#include "augsrv/control.h"
#include "augsrv/daemon.h"
#include "augsrv/options.h"

#include "augutil/log.h"

#include "augsys/utility.h" /* aug_perrinfo() */
#include "augsys/windows.h" /* GetStdHandle() */

#include "augctx/base.h"
#include "augctx/errinfo.h"

#include "augext/log.h"

#include <assert.h>
#include <setjmp.h>

static jmp_buf mark_;

static void
die_(const char* s)
{
    aug_perrinfo(aug_tlx, s, NULL);
    longjmp(mark_, 1);
}

static void
foreground_(void)
{
    int ret;
    if (-1 == aug_initservice())
        die_("aug_initservice() failed");

    ret = aug_runservice();
    aug_termservice();
    if (-1 == ret)
        die_("aug_runservice() failed");
}

static void
daemonise_(void)
{
    switch (aug_daemonise()) {
    case -1:
        die_("aug_daemonise() failed");
    case 0:
        break;
    case AUG_FAILNONE:
        foreground_();
        break;
    default:
        assert(0);
    }
}

#if defined(_WIN32)
static void
start_(void)
{
    switch (aug_start()) {
    case -1:
        die_("aug_start() failed");
    case 0:
        break;
    default:
        assert(0);
    }
}
#endif /* _WIN32 */

static void
control_(int sigtype)
{
    switch (aug_control(sigtype)) {
    case -1:
        die_("aug_control() failed");
    case 0:
        break;
    default:
        assert(0);
    }
}

static void
install_(void)
{
    switch (aug_install()) {
    case -1:
        die_("aug_install() failed");
    case 0:
        break;
    default:
        assert(0);
    }
}

static void
uninstall_(void)
{
    switch (aug_uninstall()) {
    case -1:
        die_("aug_uninstall() failed");
    case 0:
        break;
    default:
        assert(0);
    }
}

AUGSRV_API int
aug_main(int argc, char* argv[], const struct aug_service* service, void* arg)
{
    struct aug_options options;
    int daemon = 0, jmpret = setjmp(mark_);
    if (jmpret)
        return jmpret;

    aug_setservice_(service, arg);

#if defined(_WIN32)

    /* Assume that if there is no stdin handle then the process is being
       started by the Service Control Manager. */

    if (!GetStdHandle(STD_INPUT_HANDLE)) {

        /* Note: aug_readopts() will be called from the main service
           function. */

        aug_setlog(aug_tlx, aug_getdaemonlog());
        daemonise_();
        return 0;
    }
#endif /* _WIN32 */

    if (-1 == aug_readopts(&options, argc, argv))
        return 1;

    if (AUG_CMDEXIT == options.command_)
        return 0;

#if !defined(_WIN32)
    if (AUG_CMDSTART == options.command_) {

        /* Install daemon logger prior to opening log file. */

        aug_setlog(aug_tlx, aug_getdaemonlog());
        daemon = 1;
    }
#endif /* !_WIN32 */

    if (-1 == aug_readserviceconf(*options.conffile_
                                  ? options.conffile_ : NULL,
                                  options.batch_, daemon))
        die_("aug_readserviceconf() failed");

    switch (options.command_) {
    case AUG_CMDDEFAULT:
        foreground_();
        break;
    case AUG_CMDEXIT:
        assert(0);
    case AUG_CMDINSTALL:
        aug_ctxinfo(aug_tlx, "installing daemon process");
        install_();
        break;
    case AUG_CMDRECONF:
        aug_ctxinfo(aug_tlx, "re-configuring daemon process");
        control_(AUG_EVENTRECONF);
        break;
    case AUG_CMDSTART:
#if !defined(_WIN32)
        daemonise_();
#else /* _WIN32 */
        start_();
#endif /* _WIN32 */
        break;
    case AUG_CMDSTATUS:
        aug_ctxinfo(aug_tlx, "getting status of daemon process");
        control_(AUG_EVENTSTATUS);
        break;
    case AUG_CMDSTOP:
        aug_ctxinfo(aug_tlx, "stopping daemon process");
        control_(AUG_EVENTSTOP);
        break;
    case AUG_CMDTEST:
        foreground_();
        break;
    case AUG_CMDUNINSTALL:
        aug_ctxinfo(aug_tlx, "uninstalling daemon process");
        uninstall_();
        break;
    }
    return 0;
}
