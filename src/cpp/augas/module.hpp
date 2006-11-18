/* Copyright (c) 2004-2006, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#ifndef AUGAS_MODULE_HPP
#define AUGAS_MODULE_HPP

#include "augas.h"

#include "augsyspp/dlfcn.hpp"
#include "augsyspp/smartptr.hpp"

#include <string>

namespace augas {

    class module {
        const std::string name_;
        aug::dlib lib_;
        augas_termfn termfn_;
        augas_module module_;

        module(const module&);

        module&
        operator =(const module&);

    public:
        ~module() AUG_NOTHROW;

        module(const std::string& name, const char* path,
               const augas_host& host);

        void
        closesess(const augas_sess& sess) const;

        void
        opensess(augas_sess& sess) const;


        void
        event(const augas_sess& sess, int type, void* user) const;

        void
        expire(const augas_sess& sess, unsigned tid, void* user,
               unsigned& ms) const;

        void
        reconf(const augas_sess& sess) const;

        void
        closeconn(const augas_conn& conn) const;

        void
        openconn(augas_conn& conn, const char* addr,
                 unsigned short port) const;

        void
        notconn(const augas_conn& conn) const;

        void
        data(const augas_conn& conn, const char* buf, size_t size) const;

        void
        rdexpire(const augas_conn& conn, unsigned& ms) const;

        void
        wrexpire(const augas_conn& conn, unsigned& ms) const;

        void
        teardown(const augas_conn& conn) const;
    };

    typedef aug::smartptr<module> moduleptr;
}

#endif // AUGAS_MODULE_HPP