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

#ifndef AUGMARPP_TYPES_HPP
#define AUGMARPP_TYPES_HPP

#include "augmarpp/config.hpp"

#include "augnullpp.hpp"

#include "augmar/mar.h"
#include "augmar/types.h"

#include <cstring> // strlen()

namespace aug {

    class field {
        aug_field field_;
        void
        clear()
        {
            field_.name_ = 0;
            field_.value_ = 0;
            field_.size_ = 0;
        }
    public:
        field(const null_&) AUG_NOTHROW
        {
            clear();
        }
        explicit
        field(const aug_field& f)
            : field_(f)
        {
        }
        explicit
        field(const char* s)
        {
            setname(s);
            field_.value_ = 0;
            field_.size_ = 0;
        }
        field(const char* s, const void* v, unsigned n)
        {
            setname(s);
            putfieldn(v, n);
        }
        field(const char* s, const char* v)
        {
            setname(s);
            putfieldn(v);
        }
        field&
        operator =(const null_&) AUG_NOTHROW
        {
            clear();
            return *this;
        }
        void
        setname(const char* s)
        {
            field_.name_ = s;
        }
        void
        putfieldn(const void* v, unsigned n)
        {
            field_.value_ = v;
            field_.size_ = n;
        }
        void
        putfieldn(const char* v)
        {
            field_.value_ = v;
            field_.size_ = static_cast<unsigned>(strlen(v));
        }
        operator aug_field&()
        {
            return field_;
        }
        const char*
        name() const
        {
            return field_.name_;
        }
        const void*
        value() const
        {
            return field_.value_;
        }
        unsigned
        size() const
        {
            return field_.size_;
        }
        operator const aug_field&() const
        {
            return field_;
        }
    };
}

inline bool
isnull(const aug::field& f)
{
    return 0 == f.name();
}

#endif // AUGMARPP_TYPES_HPP
