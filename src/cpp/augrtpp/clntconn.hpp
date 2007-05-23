/* Copyright (c) 2004-2007, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#ifndef AUGRTPP_CLNTCONN_HPP
#define AUGRTPP_CLNTCONN_HPP

#include "augrtpp/buffer.hpp"
#include "augrtpp/conn.hpp"

namespace aug {

    class clntconn : public rwtimer_base, public conn_base {

        augas_object sock_;
        buffer buffer_;
        rwtimer rwtimer_;
        connptr conn_;

        // rwtimer_base.

        void
        do_timercb(int id, unsigned& ms);

        void
        do_setrwtimer(unsigned ms, unsigned flags);

        bool
        do_resetrwtimer(unsigned ms, unsigned flags);

        bool
        do_resetrwtimer(unsigned flags);

        bool
        do_cancelrwtimer(unsigned flags);

        // conn_base.

        augas_object&
        do_get();

        const augas_object&
        do_get() const;

        const servptr&
        do_serv() const;

        smartfd
        do_sfd() const;

        void
        do_append(const aug_var& var);

        void
        do_append(const void* buf, size_t size);

        bool
        do_accepted(const aug_endpoint& ep);

        void
        do_connected(const aug_endpoint& ep);

        bool
        do_process(unsigned short events);

        void
        do_shutdown();

        void
        do_teardown();

        bool
        do_authcert(const char* subject, const char* issuer);

        const endpoint&
        do_peername() const;

        sockstate
        do_state() const;

    public:
        ~clntconn() AUG_NOTHROW;

        clntconn(const servptr& serv, void* user, timers& timers,
                 const char* host, const char* port);
    };
}

#endif // AUGRTPP_CLNTCONN_HPP