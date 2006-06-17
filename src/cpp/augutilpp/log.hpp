/* Copyright (c) 2004-2006, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#ifndef AUGUTILPP_LOG_HPP
#define AUGUTILPP_LOG_HPP

#include "augutilpp/config.hpp"

#include "augsyspp/exception.hpp"

#include "augutil/log.h"

#include "augsys/defs.h" // AUG_MAXLINE

#include <string>

namespace aug {

    inline void
    vformatlog(char* buf, size_t& n, int logLevel, const char* format,
               va_list args)
    {
        if (-1 == aug_vformatlog(buf, &n, logLevel, format, args))
            error("aug_vformatlog() failed");
    }

    inline void
    formatlog(char* buf, size_t& n, int logLevel, const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        int ret(aug_vformatlog(buf, &n, logLevel, format, args));
        va_end(args);
        if (-1 == ret)
            error("aug_vformatlog() failed");
    }

    inline std::string
    vformatlog(int logLevel, const char* format, va_list args)
    {
        char buf[AUG_MAXLINE];
        size_t n(sizeof(buf));
        vformatlog(buf, n, logLevel, format, args);
        return std::string(buf, n);
    }

    inline std::string
    formatlog(int logLevel, const char* format, ...)
    {
        char buf[AUG_MAXLINE];
        size_t n(sizeof(buf));

        va_list args;
        va_start(args, format);
        int ret(aug_vformatlog(buf, &n, logLevel, format, args));
        va_end(args);
        if (-1 == ret)
            error("aug_vformatlog() failed");

        return std::string(buf, n);
    }
}

#endif // AUGUTILPP_LOG_HPP
