/* Copyright (c) 2004-2006, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#ifndef AUGSRV_BASE_H
#define AUGSRV_BASE_H

#include "augsrv/config.h"
#include "augsrv/types.h"

#if defined(AUGSRV_BUILD)
AUGSRV_EXTERN void
aug_setservice_(const struct aug_service* service);
#endif /* AUGSRV_BUILD */

AUGSRV_API const char*
aug_getserviceopt(enum aug_option opt);

AUGSRV_API int
aug_readserviceconf(const char* conffile, int daemon);

AUGSRV_API int
aug_initservice(void);

AUGSRV_API int
aug_runservice(void);

AUGSRV_API void
aug_exitservice(int status);

AUGSRV_API int
aug_eventin(void);

AUGSRV_API int
aug_eventout(void);

#endif /* AUGSRV_BASE_H */
