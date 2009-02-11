/* Copyright (c) 2004-2009, Mark Aylett <mark.aylett@gmail.com>
   See the file COPYING for copying permission.
*/
#ifndef AUGSERV_MAIN_H
#define AUGSERV_MAIN_H

#include "augserv/config.h"

struct aug_serv;

/**
 * On Windows, the Service Manager calls the service entry point on a separate
 * thread - automatic variables on the main thread's stack will not be visible
 * from the service thread.  A shallow copy of the service structure is,
 * therefore, performed by aug_main().
 *
 * @return #EXIT_SUCCESS or #EXIT_FAILURE.
 */

AUGSERV_API int
aug_main(int argc, char* argv[], const struct aug_serv* serv);

#endif /* AUGSERV_MAIN_H */