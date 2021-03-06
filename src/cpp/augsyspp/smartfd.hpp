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
#ifndef AUGSYSPP_SMARTFD_HPP
#define AUGSYSPP_SMARTFD_HPP

#include "augsyspp/types.hpp"

#include "augctxpp/exception.hpp"
#include "augctxpp/utility.hpp" // perrinfo()

#include <algorithm>            // swap()
#include <cassert>

namespace aug {

    template <typename traitsT>
    struct autoclose_proxy {
        basic_ref<traitsT> ref_;
        void (*close_)(basic_ref<traitsT> ref_);
        autoclose_proxy(basic_ref<traitsT> ref,
                        void (*close)(basic_ref<traitsT>))
            : ref_(ref),
              close_(close)
        {
        }
    };

    template <typename traitsT>
    class autoclose {
        basic_ref<traitsT> ref_;
        void (*close_)(basic_ref<traitsT> ref_);
    public:
        ~autoclose()
        {
            if (null != ref_ && close_)
                try {
                    close_(ref_);
                } AUG_PERRINFOCATCH;
        }
        autoclose(const null_&)
            : ref_(null),
              close_(0)
        {
        }
        autoclose(basic_ref<traitsT> ref, void (*close)(basic_ref<traitsT>))
            : ref_(ref),
              close_(close)
        {
        }
        autoclose(autoclose& rhs)
            : ref_(rhs.release()),
              close_(rhs.close_)
        {
        }
        autoclose(const autoclose_proxy<traitsT>& rhs)
            : ref_(rhs.ref_),
              close_(rhs.close_)
        {
        }
        autoclose&
        operator =(autoclose& rhs)
        {
            reset(rhs.release(), rhs.close_);
            return *this;
        }
        autoclose&
        operator =(const autoclose_proxy<traitsT>& rhs)
        {
            reset(rhs.ref_, rhs.close_);
            return *this;
        }
        void
        reset(basic_ref<traitsT> ref, void (*close)(basic_ref<traitsT>))
        {
            if (ref != ref_ && null != ref_ && close_)
                close_(ref_);
            ref_ = ref;
            close_ = close;
        }
        basic_ref<traitsT>
        release()
        {
            basic_ref<traitsT> tmp(ref_);
            ref_ = null;
            close_ = 0;
            return tmp;
        }
        operator autoclose_proxy<traitsT>()
        {
            return autoclose_proxy<traitsT>(release(), close_);
        }
        operator basic_ref<traitsT>() const
        {
            return ref_;
        }
        typename traitsT::ref
        get() const
        {
            return ref_.get();
        }
    };

    template <typename traitsT>
    class autoclose2 {
        struct proxy {
            autoclose_proxy<traitsT> first_;
            autoclose_proxy<traitsT> second_;
            proxy(autoclose_proxy<traitsT> first,
                  autoclose_proxy<traitsT> second)
                : first_(first),
                  second_(second)
            {
            }
        };
        autoclose<traitsT> first_;
        autoclose<traitsT> second_;
    public:
        autoclose2(basic_ref<traitsT> first, basic_ref<traitsT> second,
                   void (*close)(basic_ref<traitsT>))
            : first_(first, close),
              second_(second, close)
        {
        }
        autoclose2(autoclose2& rhs)
            : first_(rhs.first_),
              second_(rhs.second_)
        {
        }
        autoclose2(const proxy& rhs)
            : first_(rhs.first_),
              second_(rhs.second_)
        {
        }
        autoclose2&
        operator =(autoclose2& rhs)
        {
            first_ = rhs.first_;
            second_ = rhs.second_;
            return *this;
        }
        autoclose2&
        operator =(const proxy& rhs)
        {
            first_.reset(rhs.first_);
            second_.reset(rhs.second_);
            return *this;
        }
        void
        release()
        {
            first_.release();
            second_.release();
        }
        operator proxy()
        {
            return proxy(first_, second_);
        }
        autoclose<traitsT>&
        operator [](unsigned i)
        {
            assert(i < 2);
            return 0 == i ? first_ : second_;
        }
        const autoclose<traitsT>&
        operator [](unsigned i) const
        {
            assert(i < 2);
            return 0 == i ? first_ : second_;
        }
    };

    typedef autoclose<fd_traits> autofd;
    typedef autoclose<sd_traits> autosd;
    typedef autoclose<sd_traits> automd;

    typedef autoclose2<fd_traits> autofds;
    typedef autoclose2<sd_traits> autosds;
    typedef autoclose2<sd_traits> automds;
}

template <typename traitsT>
bool
isnull(const aug::autoclose<traitsT>& ac)
{
    return ac.get() == traitsT::bad();
}

#endif // AUGSYSPP_SMARTFD_HPP
