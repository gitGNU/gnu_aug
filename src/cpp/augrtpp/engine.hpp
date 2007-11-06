/* Copyright (c) 2004-2007, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#ifndef AUGRTPP_ENGINE_HPP
#define AUGRTPP_ENGINE_HPP

#include "augrtpp/session.hpp"

#include "augnetpp/nbfile.hpp"

#include "augmod.h"

namespace aug {

    class AUGRTPP_API sslctx;
    class timers;

    namespace detail {
        struct engineimpl;
    }

    class AUGRTPP_API enginecb_base {
        virtual void
        do_reconf() = 0;

    public:
        virtual
        ~enginecb_base() AUG_NOTHROW;

        void
        reconf()
        {
            do_reconf();
        }
    };

    class AUGRTPP_API engine {

        detail::engineimpl* const impl_;

        engine(const engine& rhs);

        engine&
        operator =(const engine& rhs);

    public:
        ~engine() AUG_NOTHROW;

        engine(const smartfd& eventrd, const smartfd& eventwr, timers& timers,
               enginecb_base& cb);

        void
        clear();

        void
        insert(const std::string& name, const sessionptr& session,
               const char* groups);

        void
        cancelinactive();

        void
        run(bool stoponerr);

        // Thread-safe host interface.

        void
        reconfall();

        void
        stopall();

        void
        post(const char* sname, const char* to, const char* type,
             const aug_var* var);

        void
        post(const char* sname, const char* to, const char* type,
             const aug_var& var)
        {
            post(sname, to, type, &var);
        }
        void
        post(const char* sname, const char* to, const char* type,
             const null_&)
        {
            post(sname, to, type, 0);
        }

        // Thread-unsafe host interface.

        void
        dispatch(const char* sname, const char* to, const char* type,
                 const void* user, size_t size);

        void
        shutdown(augmod_id cid, unsigned flags);

        augmod_id
        tcpconnect(const char* sname, const char* host, const char* port,
                   void* user);

        augmod_id
        tcplisten(const char* sname, const char* host, const char* port,
                  void* user);

        void
        send(augmod_id cid, const void* buf, size_t len);

        void
        sendv(augmod_id cid, const aug_var& var);

        void
        setrwtimer(augmod_id cid, unsigned ms, unsigned flags);

        bool
        resetrwtimer(augmod_id cid, unsigned ms, unsigned flags);

        bool
        cancelrwtimer(augmod_id cid, unsigned flags);

        augmod_id
        settimer(const char* sname, unsigned ms, const aug_var* var);

        augmod_id
        settimer(const char* sname, unsigned ms, const aug_var& var)
        {
            return settimer(sname, ms, &var);
        }
        augmod_id
        settimer(const char* sname, unsigned ms, const null_&)
        {
            return settimer(sname, ms, 0);
        }

        bool
        resettimer(augmod_id tid, unsigned ms);

        bool
        canceltimer(augmod_id tid);

        void
        setsslclient(augmod_id cid, sslctx& ctx);

        void
        setsslserver(augmod_id cid, sslctx& ctx);

        bool
        stopping() const;
    };
}

#endif // AUGRTPP_ENGINE_HPP
