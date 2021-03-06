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
#define MOD_BUILD
#include "augmodpp.hpp"

#include "augnetpp.hpp"
#include "augutilpp.hpp"
#include "augmarpp.hpp"
#include "augsyspp.hpp"

#include <map>
#include <sstream>

using namespace aug;
using namespace mod;
using namespace std;

namespace {

    enum tmtz { TMLOCAL, TMUTC };

    struct tmevent : mpool_ops {
        const mod_id id_;
        const string name_, spec_;
        const tmtz tz_;
        aug_tmspec tmspec_;
        tmevent(mod_id id, const string& name, const string& spec, tmtz tz)
            : id_(id),
              name_(name),
              spec_(spec),
              tz_(tz)
        {
        }
        tmevent(const string& name, const string& spec, tmtz tz)
            : id_(aug_nextid()),
              name_(name),
              spec_(spec),
              tz_(tz)
        {
        }
    };

    typedef smartptr<tmevent> tmeventptr;
    typedef multimap<aug_time, tmeventptr> tmqueue;

    string
    tmstring(const tm& local)
    {
        char buf[256];
        strftime(buf, sizeof(buf), "%b %d %H:%M:%S", &local);
        return buf;
    }

    void
    pushevent(tmqueue& q, aug_time now, const tmeventptr& ptr)
    {
        tm tm;
        if (TMUTC == ptr->tz_) {

            if (!aug_nexttime(aug_gmtime(&now, &tm), &ptr->tmspec_))
                return;

            q.insert(make_pair(now = aug_timegm(&tm), ptr));

        } else {

            if (!aug_nexttime(aug_localtime(&now, &tm), &ptr->tmspec_))
                return;

            q.insert(make_pair(now = aug_timelocal(&tm), ptr));
        }

        aug_ctxinfo(aug_tlx, "event [%s] scheduled for %s",
                    ptr->name_.c_str(),
                    tmstring(*aug_localtime(&now, &tm)).c_str());
    }

    void
    pushevent(tmqueue& q, aug_time now, const string& name, tmtz tz)
    {
        const char* tmspecs
            (mod::getenv(string("session.sched.event.").append(name)
                          .append(TMUTC == tz ? ".utc" : ".local")
                          .c_str()));
        if (!tmspecs)
            return;

        istringstream is(tmspecs);
        string spec;
        while (is >> spec) {

            tmeventptr ptr(new (tlx) tmevent(name, spec, tz));
            if (aug_strtmspec(&ptr->tmspec_, spec.c_str()))
                pushevent(q, now, ptr);
        }
    }

    void
    pushevents(tmqueue& q, aug_time now)
    {
        const char* events(mod::getenv("session.sched.events"));
        if (!events)
            return;

        istringstream is(events);
        string name;
        while (is >> name) {
            pushevent(q, now, name, TMLOCAL);
            pushevent(q, now, name, TMUTC);
        }
    }

    void
    eraseevent(tmqueue& q, mod_id id)
    {
        tmqueue::iterator it(q.begin()), end(q.end());
        for (; it != end; ++it)
            if (it->second->id_ == id) {
                q.erase(it);
                break;
            }
    }

    unsigned
    timerms(const tmqueue& q, const aug_timeval& tv)
    {
        if (q.empty())
            return 0;

        tmqueue::const_iterator next(q.begin());
        aug_timeval expiry = { next->first, 0 };
        tvsub(expiry, tv);
        unsigned ms(tvtoms(expiry));
        return ms < 60000 ? ms : 60000;
    }

    ostream&
    toxml(ostream& os, const tmqueue& q, unsigned offset, unsigned max_)
    {
        offset = AUG_MIN(offset, q.size());

        os << "<result><events offset=\"" << offset
           << "\" total=\"" << static_cast<unsigned>(q.size()) << "\">";
        tmqueue::const_iterator it(q.begin()), end(q.end());
        for (advance(it, offset); it != end && max_; ++it, --max_) {
            tm tm;
            os << "<event id=\"" << it->second->id_
               << "\" name=\"" << it->second->name_
               << "\" spec=\"" << it->second->spec_ << "\" tz=\""
               << (TMUTC == it->second->tz_ ? "utc" : "local")
               << "\">" << tmstring(*aug_localtime(&it->first, &tm))
               << "</event>";
        }
        os << "</events></result>";
        return os;
    }

    class sched : public basic_session<sched>, public mpool_ops {
        const string sname_;
        mod_id timer_;
        tmqueue queue_;
        void
        checkexpired(const aug_timeval& tv)
        {
            aug_time now(tv.tv_sec);

            while (!queue_.empty() && queue_.begin()->first <= now) {

                tmeventptr ptr(queue_.begin()->second);
                queue_.erase(queue_.begin());

                // Inform interested modules of expired event.

                mod_post("sched-client", ptr->name_.c_str(), 0, 0);
                pushevent(queue_, now, ptr);
            }
        }
        void
        settimer(const aug_timeval& tv)
        {
            checkexpired(tv);

            unsigned ms(timerms(queue_, tv));
            if (ms) {
                if (timer_)
                    resettimer(timer_, ms);
                else
                    timer_ = mod::settimer(ms, null);
                aug_ctxinfo(aug_tlx, "next expiry in %d ms", ms);
            } else if (timer_) {
                canceltimer(timer_);
                timer_ = 0;
            }
        }
        void
        delevent(const map<string, string>& params)
        {
            mod_id id(getvalue<mod_id>(params, "id"));

            aug_ctxinfo(aug_tlx, "deleting event: id=[%d]", (int)id);
            eraseevent(queue_, id);

            aug_timeval tv;
            gettimeofday(getclock(aug_tlx), tv);
            settimer(tv);
        }
        void
        putevent(const map<string, string>& params)
        {
            mod_id id(getvalue<mod_id>(params, "id"));

            if (id) {
                aug_ctxinfo(aug_tlx, "updating event: id=[%d]", (int)id);
                eraseevent(queue_, id);
            } else {
                aug_ctxinfo(aug_tlx, "inserting new event");
                id = aug_nextid();
            }

            string name(getvalue<string>(params, "name"));
            string spec(getvalue<string>(params, "spec"));

            tmtz tz(TMUTC);
            if (getvalue<string>(params, "tz") == "local")
                tz = TMLOCAL;

            aug_timeval tv;
            gettimeofday(getclock(aug_tlx), tv);

            tmeventptr ptr(new (tlx) tmevent(id, name, spec, tz));
            if (aug_strtmspec(&ptr->tmspec_, spec.c_str()))
                pushevent(queue_, tv.tv_sec, ptr);

            settimer(tv);
        }
        void
        respond(mod_id id, const char* from, const char* type, blobref blob)
        {
            size_t size;
            const char* content(static_cast<const char*>
                                (aug::getblobdata(blob, size)));

            map<string, string> params;
            unpackurl(content, content + size,
                      inserter(params, params.begin()));

            marptr mar(createmar(getmpool(aug_tlx)));
            putfieldp(mar, "Cache-Control", "no-cache");
            putfieldp(mar, "Content-Type", "text/xml");

            if (0 == strcmp(type, "service/reconf")) {

                reconfall();
                setcontent(mar, "<message type=\"info\">"
                           "re-configured</message>");
            } else {

                if (0 == strcmp(type, "service/sched/delevent")) {
                    delevent(params);
                } else if (0 == strcmp(type, "service/sched/putevent")) {
                    putevent(params);
                } else if (0 != strcmp(type, "service/sched/events")) {
                    aug_ctxwarn(aug_tlx, "event [%s] ignored", type);
                    return;
                }

                unsigned offset(getvalue<unsigned>(params, "offset"));
                unsigned max_(getvalue<unsigned>(params, "max"));

                omarstream os(mar);
                toxml(os, queue_, offset, max_);
            }

            dispatch(from, "result", id, mar.base());
        }
        explicit
        sched(const string& sname)
            : sname_(sname),
              timer_(0)
        {
        }
    public:
        ~sched() AUG_NOTHROW
        {
            // Deleted from base.
        }
        mod_bool
        start()
        {
            reconf();
            return MOD_TRUE;
        }
        void
        stop()
        {
        }
        void
        reconf()
        {
            aug_timeval tv;
            gettimeofday(getclock(aug_tlx), tv);

            queue_.clear();
            pushevents(queue_, tv.tv_sec);
            settimer(tv);
        }
        void
        event(const char* from, const char* type, mod_id id, objectref ob)
        {
            aug_ctxinfo(aug_tlx, "event [%s] triggered", type);

            marptr mar(object_cast<aug_mar>(ob));
            if (null == mar)
                return;

            const void* value;
            getfieldp(mar, "Content-Type", value);
            if (!value || 0 != strcmp(static_cast<const char*>(value),
                                      "application/x-www-form-urlencoded"))
                return;

            respond(id, from, type, object_cast<aug_blob>(mar));
        }
        void
        closed(mod_handle& sock)
        {
            aug_assign(sock.ob_, 0);
        }
        void
        teardown(mod_handle& sock)
        {
            mod::shutdown(sock, 0);
        }
        mod_bool
        accepted(mod_handle& sock, const char* name)
        {
            return MOD_TRUE;
        }
        void
        connected(mod_handle& sock, const char* name)
        {
        }
        mod_bool
        auth(mod_handle& sock, const char* subject, const char* issuer)
        {
            return MOD_TRUE;
        }
        void
        recv(mod_handle& sock, const void* buf, size_t len)
        {
        }
        void
        mrecv(const char* node, unsigned sess, unsigned short type,
              const void* buf, size_t len)
        {
        }
        void
        error(mod_handle& sock, const char* desc)
        {
        }
        void
        rdexpire(mod_handle& sock, unsigned& ms)
        {
        }
        void
        wrexpire(mod_handle& sock, unsigned& ms)
        {
        }
        void
        expire(mod_handle& timer, unsigned& ms)
        {
            aug_timeval tv;
            gettimeofday(getclock(aug_tlx), tv);

            checkexpired(tv);

            ms = timerms(queue_, tv);
            aug_ctxinfo(aug_tlx, "next expiry in %d ms", ms);
        }
        static sessionptr
        create(const char* sname)
        {
            return attach(new (tlx) sched(sname));
        }
    };

    typedef basic_module<basic_factory<sched> > module;
}

MOD_ENTRYPOINTS(module::init, module::term, module::create)
