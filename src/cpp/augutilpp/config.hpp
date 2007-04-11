/* Copyright (c) 2004-2007, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#ifndef AUGUTILPP_CONFIG_HPP
#define AUGUTILPP_CONFIG_HPP

#if !defined(_WIN32)
# define AUGUTILPP_EXPORT
# define AUGUTILPP_IMPORT
#else // _WIN32
# define AUGUTILPP_EXPORT __declspec(dllexport)
# define AUGUTILPP_IMPORT __declspec(dllimport)
#endif // _WIN32

#if defined(DLL_EXPORT) || defined(_WINDLL)
# define AUGUTILPP_SHARED
#endif // DLL_EXPORT || _WINDLL

#if !defined(AUGUTILPP_SHARED)
# define AUGUTILPP_API
#else // AUGUTILPP_SHARED
# if !defined(AUGUTILPP_BUILD)
#  define AUGUTILPP_API AUGUTILPP_IMPORT
# else // AUGUTILPP_BUILD
#  define AUGUTILPP_API AUGUTILPP_EXPORT
# endif // AUGUTILPP_BUILD
#endif // AUGUTILPP_SHARED

#if !defined(AUG_NOTHROW)
# define AUG_NOTHROW throw()
#endif // !AUG_NOTHROW

#if HAVE_CONFIG_H
# ifndef AUGCONFIG_H
# define AUGCONFIG_H
#  include "augconfig.h"
# endif // AUGCONFIG_H
#endif // HAVE_CONFIG_H

#endif // AUGUTILPP_CONFIG_HPP
