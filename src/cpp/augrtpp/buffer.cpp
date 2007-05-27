/* Copyright (c) 2004-2007, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#define AUGRTPP_BUILD
#include "augrtpp/buffer.hpp"
#include "augsys/defs.h"

AUG_RCSID("$Id$");

#include "augutilpp/var.hpp"

#include <string>

using namespace aug;
using namespace std;

namespace {

    struct vectype : basic_vartype<vecarg> {
        static void
        destroy(arg_type* arg)
        {
            arg->begin_ = arg->end_ = 0;
        }
        static const void*
        buf(arg_type& arg, size_t& size)
        {
            size = arg.end_ - arg.begin_;
            return &arg.vec_[arg.begin_];
        }
        static const void*
        buf(arg_type& arg)
        {
            return &arg.vec_[arg.begin_];
        }
    };

    struct copyarg {
        size_t size_;
        char buf_[1];
    };

    struct copytype : basic_vartype<copyarg> {
        static void
        destroy(arg_type* arg)
        {
            free(arg);
        }
        static const void*
        buf(arg_type& arg, size_t& size)
        {
            size = arg.size_;
            return arg.buf_;
        }
        static const void*
        buf(arg_type& arg)
        {
            return arg.buf_;
        }
    };
}

buffer::~buffer() AUG_NOTHROW
{
}

buffer::buffer(size_t size)
    : arg_(size),
      usevec_(true)
{
}

void
buffer::append(const aug_var& var)
{
    appendwriter(writer_, var);
    usevec_ = false;
}

void
buffer::append(const void* buf, size_t len)
{
    if (usevec_) {

        if (empty()) {
            aug_var var;
            appendwriter(writer_, bindvar<vectype>(var, arg_));
        }

        // Grow buffer if needed.

        if (arg_.vec_.size() - arg_.end_ < len)
            arg_.vec_.resize(arg_.end_ + len);

        memcpy(&arg_.vec_[arg_.end_], buf, len);
        arg_.end_ += len;

    } else {

        copyarg* arg(static_cast<copyarg*>(malloc(sizeof(copyarg)
                                                  + len - 1)));
        arg->size_ = len;
        memcpy(arg->buf_, buf, len);

        aug_var var;
        appendwriter(writer_, bindvar<copytype>(var, *arg));
    }
}

bool
buffer::writesome(fdref ref)
{
    aug::writesome(writer_, ref);
    if (writer_.empty()) {
        usevec_ = true;
        return true;
    }
    return false;
}
