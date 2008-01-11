/* Copyright (c) 2004-2007, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#ifndef AUGSYS_DEFS_H
#define AUGSYS_DEFS_H

#include <stddef.h>

/**
 * @defgroup Constants Constants
 * @defgroup Macros Macros
 */

/**
 * @ingroup Macros
 *
 * Calculate offset of member @a m in structure @a s.
 *
 * The offsetof() macro can only be used for PODs.  GCC will emit the
 * following warning when used on non-POD types:
 *
 * warning: (perhaps the 'offsetof' macro was used incorrectly)
 */

#if !defined(offsetof)
# define offsetof(s, m) (size_t)&(((s*)0)->m)
#endif /* !offsetof */

#if !defined(AUG_MAXLINE)
# define AUG_MAXLINE 1024
#endif /* !AUG_MAXLINE */

/**
 * @defgroup MinMax Min Max
 *
 * @ingroup Macros
 *
 * Portable min and max macros.
 *
 * @{
 */

#define AUG_MIN(a, b) ((a) <= (b) ? (a) : (b))
#define AUG_MAX(a, b) ((a) >= (b) ? (a) : (b))

/** @} */

/**
 * Used to tag literal strings for future message catalog support.
 *
 * @ingroup Macros
 */

#define AUG_MSG(x) x

/**
 * @defgroup Stringify Stringify
 *
 * @ingroup Macros
 *
 * Stringify macro.
 *
 * @{
 */

#define AUG_MKSTR_(x) #x
#define AUG_MKSTR(x)  AUG_MKSTR_(x)

/** @} */

/**
 * @defgroup ReturnCodes Return Codes
 *
 * @ingroup Constants
 *
 * Standard integer return codes.
 *
 * @{
 */

#define AUG_RETOK      0
#define AUG_RETERROR (-1)
#define AUG_RETNONE  (-2)
#define AUG_RETINTR  (-3)

/** @} */

#if !defined(__GNUC__)
# define AUG_RCSID(x)                           \
    static const char rcsid[] = x
#else /* __GNUC__ */
# define AUG_RCSID(x)                                   \
    static const char rcsid[] __attribute__((used)) = x
#endif /* __GNUC__ */

#if !defined(__GNUC__)
# define AUG_DLLMAIN(init, term)                                \
    BOOL WINAPI                                                 \
    DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)  \
    {                                                           \
        switch (reason) {                                       \
        case DLL_PROCESS_ATTACH:                                \
            init();                                             \
            break;                                              \
        case DLL_THREAD_ATTACH:                                 \
            break;                                              \
        case DLL_THREAD_DETACH:                                 \
            break;                                              \
        case DLL_PROCESS_DETACH:                                \
            term();                                             \
            break;                                              \
        }                                                       \
        return TRUE;                                            \
    }
#else /* __GNUC__ */
# define AUG_DLLMAIN(init, term)                \
    static void __attribute__ ((constructor))   \
    aug_dllinit_(void)                          \
    {                                           \
        init();                                 \
    }                                           \
    static void __attribute__ ((destructor))    \
    aug_dllterm_(void)                          \
    {                                           \
        term();                                 \
    }
#endif /* __GNUC__ */

#endif /* AUGSYS_DEFS_H */
