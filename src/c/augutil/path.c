/*
  Copyright (c) 2004, 2005, 2006, 2007, 2008, 2009 Mark Aylett <mark.aylett@gmail.com>

  This file is part of Aug written by Mark Aylett.

  Aug is released under the GPL with the additional exemption that compiling,
  linking, and/or using OpenSSL is allowed.

  Aug is free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free Software
  Foundation; either version 2 of the License, or (at your option) any later
  version.

  Aug is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
  details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc., 51
  Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#undef __STRICT_ANSI__     /* _fullpath() */
#define AUGUTIL_BUILD
#include "augutil/path.h"
#include "augctx/defs.h"

AUG_RCSID("$Id$");

#include "augsys/limits.h" /* AUG_PATH_MAX */
#include "augsys/windows.h"
#include "augsys/unistd.h" /* chdir() */

#include "augctx/base.h"
#include "augctx/errinfo.h"
#include "augctx/string.h"

#include <ctype.h>         /* isalpha() */
#include <errno.h>
#include <stdlib.h>        /* realpath() */
#include <string.h>        /* strlen() */

#if !defined(_WIN32)
# if HAVE_ALLOCA_H
#  include <alloca.h>
# endif /* HAVE_ALLOCA_H */
# define IS_DIRSEP_(ch) ((ch) == '/')
#else /* _WIN32 */
# include <malloc.h>       /* alloca() */
# define IS_DIRSEP_(ch) ((ch) == '/' || (ch) == '\\')
#endif /* _WIN32 */

AUGUTIL_API const char*
aug_basename(const char* path)
{
    const char* base;

#if defined(_WIN32)
    if (isalpha(path[0]) && ':' == path[1])
        path += 2;
#endif /* _WIN32 */

    for (base = path; *path; ++path)
        if (IS_DIRSEP_(*path))
            base = path + 1;

    return base;
}

AUGUTIL_API aug_result
aug_chdir(const char* path)
{
#if !defined(_WIN32)
    if (-1 == chdir(path))
#else /* _WIN32 */
    if (-1 == _chdir(path))
#endif /* _WIN32 */
        return aug_setposixerrinfo(aug_tlerr, __FILE__, __LINE__, errno);

    return AUG_SUCCESS;
}

AUGUTIL_API char*
aug_getcwd(char* dst, size_t size)
{
#if !defined(_WIN32)
    if (!getcwd(dst, size)) {
#else /* _WIN32 */
    if (!_getcwd(dst, (int)size)) {
#endif /* _WIN32 */
        aug_setposixerrinfo(aug_tlerr, __FILE__, __LINE__, errno);
        return NULL;
    }
    return dst;
}

AUGUTIL_API char*
aug_gethome(char* dst, size_t size)
{
    const char* home = getenv("HOME");
#if defined(_WIN32)
    if (!home)
        home = getenv("APPDATA");
#endif /* _WIN32 */
    if (!home) {
        aug_seterrinfo(aug_tlerr, __FILE__, __LINE__, "aug", AUG_EEXIST,
                       AUG_MSG("failed to determine home directory"));
        return NULL;
    }
    aug_strlcpy(dst, home, size);
    return dst;
}

AUGUTIL_API char*
aug_gettmp(char* dst, size_t size)
{
#if !defined(_WIN32)
    aug_strlcpy(dst, "/tmp", size);
#else /* _WIN32 */
    char buf[MAX_PATH + 1]; /* Ensure required buffer space. */
    if (0 == GetTempPath(sizeof(buf), buf)) {
        aug_setwin32errinfo(aug_tlerr, __FILE__, __LINE__, GetLastError());
        return NULL;
    }
    aug_strlcpy(dst, buf, size);
#endif /* _WIN32 */
    return dst;
}

AUGUTIL_API aug_bool
aug_isabs(const char* path)
{
    /* Absolute if path starts with root directory. */

    if (IS_DIRSEP_(*path))
        return AUG_TRUE;

#if defined(_WIN32)

    /* C:/ */
    /* 012 */

    if (isalpha(path[0]) && ':' == path[1] && IS_DIRSEP_(path[2]))
        return AUG_TRUE;

#endif /* _WIN32 */

    /* Otherwise false. */

    return AUG_FALSE;
}

AUGUTIL_API char*
aug_abspath(char* dst, const char* dir, const char* path, size_t size)
{
    if (aug_isabs(path)) {

        /* Already absolute. */

        aug_strlcpy(dst, path, size);

    } else {

        dst = aug_joinpath(dst, dir, path, size);
    }
    return dst;
}

AUGUTIL_API char*
aug_joinpath(char* dst, const char* dir, const char* path, size_t size)
{
    char* ptr = dst;
    size_t pathlen = strlen(path);

    /* The directory part is optional. */

    if (dir && '\0' != *dir) {

        size_t dirlen = strlen(dir);

        /* Trim trailing separator from directory part. */

        if (IS_DIRSEP_(dir[dirlen - 1]))
            --dirlen;

        if (size < dirlen + pathlen + 2) {
            aug_seterrinfo(aug_tlerr, __FILE__, __LINE__, "aug", AUG_ELIMIT,
                           AUG_MSG("buffer size exceeded"));
            return NULL;
        }

        /* Directory part. */

        memcpy(ptr, dir, dirlen);
        ptr[dirlen] = '/';
        ptr += dirlen + 1;

    } else if (size < pathlen + 1) {

        aug_seterrinfo(aug_tlerr, __FILE__, __LINE__, "aug", AUG_ELIMIT,
                       AUG_MSG("buffer size exceeded"));
        return NULL;
    }

    /* Append path. */

    strcpy(ptr, path);
    return dst;
}

AUGUTIL_API char*
aug_realpath(char* dst, const char* src, size_t size)
{
#if !defined(_WIN32)
    int pathmax;
    char* buf;

    /* FIXME: The following sequence attempts to provide a safe implementation
       of realpath().  Verify that this is indeed the case. */

    if (-1 == (pathmax = pathconf(src, _PC_PATH_MAX))) {
        aug_setposixerrinfo(aug_tlerr, __FILE__, __LINE__, errno);
        return NULL;
    }

    if (!(buf = alloca(pathmax + 1))) {
        aug_setposixerrinfo(aug_tlerr, __FILE__, __LINE__, ENOMEM);
        return NULL;
    }

    if (!realpath(src, buf)) {
        aug_setposixerrinfo(aug_tlerr, __FILE__, __LINE__, errno);
        return NULL;
    }

    if (size <= strlen(buf)) {
        aug_seterrinfo(aug_tlerr, __FILE__, __LINE__, "aug", AUG_ELIMIT,
                       AUG_MSG("buffer size exceeded"));
        return NULL;
    }

    aug_strlcpy(dst, buf, size);
    return dst;
#else /* _WIN32 */
    if (!_fullpath(dst, src, size)) {
        aug_setposixerrinfo(aug_tlerr, __FILE__, __LINE__, errno);
        return NULL;
    }
    return dst;
#endif /* _WIN32 */
}
