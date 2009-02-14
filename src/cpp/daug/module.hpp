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
#ifndef DAUG_MODULE_HPP
#define DAUG_MODULE_HPP

#include "augmod.h"

#include "augsyspp/dlfcn.hpp"
#include "augsyspp/smartptr.hpp"

#include "augctxpp/mpool.hpp"

#include "augabipp.hpp"

#include <string>

namespace daug {

    class module : public aug::mpool_ops {
        const std::string name_;
        aug::dlib lib_;
        mod_termfn termfn_;
        mod_module module_;

        module(const module&);

        module&
        operator =(const module&);

    public:
        ~module() AUG_NOTHROW;

        module(const std::string& name, const char* path,
               const mod_host& host,
               void (*teardown)(const mod_handle*));

        void
        stop() const AUG_NOTHROW;

        bool
        start(mod_session& session) const AUG_NOTHROW;

        void
        reconf() const AUG_NOTHROW;

        void
        event(const char* from, const char* type,
              aug::objectref ob) const AUG_NOTHROW;

        void
        closed(const mod_handle& sock) const AUG_NOTHROW;

        void
        teardown(const mod_handle& sock) const AUG_NOTHROW;

        bool
        accepted(mod_handle& sock, const char* name) const AUG_NOTHROW;

        void
        connected(mod_handle& sock, const char* name) const AUG_NOTHROW;

        bool
        auth(const mod_handle& sock, const char* subject,
             const char* issuer) const AUG_NOTHROW;

        void
        recv(const mod_handle& sock, const char* buf,
             size_t size) const AUG_NOTHROW;

        void
        error(const mod_handle& sock, const char* desc) const AUG_NOTHROW;

        void
        rdexpire(const mod_handle& sock, unsigned& ms) const AUG_NOTHROW;

        void
        wrexpire(const mod_handle& sock, unsigned& ms) const AUG_NOTHROW;

        void
        expire(const mod_handle& timer, unsigned& ms) const AUG_NOTHROW;
    };

    typedef aug::smartptr<module> moduleptr;
}

#endif // DAUG_MODULE_HPP
