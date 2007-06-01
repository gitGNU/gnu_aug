/* Copyright (c) 2004-2007, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#define DAUG_BUILD
#include "daug/session.hpp"
#include "augsys/defs.h"

AUG_RCSID("$Id$");

#include "augsys.h"

#include <stack>

using namespace aug;
using namespace augrt;
using namespace std;

namespace {
    stack<const augrt_session*> stack_;
    struct scoped_frame {
        ~scoped_frame() AUG_NOTHROW
        {
            stack_.pop();
        }
        explicit
        scoped_frame(const augrt_session* session)
        {
            stack_.push(session);
        }
    };
}

augrt_session&
session::do_get() AUG_NOTHROW
{
    return session_;
}

const augrt_session&
session::do_get() const AUG_NOTHROW
{
    return session_;
}

bool
session::do_active() const AUG_NOTHROW
{
    return active_;
}

bool
session::do_start() AUG_NOTHROW
{
    scoped_frame frame(&session_);
    active_ = true; // Functions may be called during initialisation.
    return active_ = module_->start(session_);
}

void
session::do_reconf() const AUG_NOTHROW
{
    scoped_frame frame(&session_);
    module_->reconf();
}

void
session::do_event(const char* from, const char* type, const void* user,
                  size_t size) const AUG_NOTHROW
{
    scoped_frame frame(&session_);
    module_->event(from, type, user, size);
}

void
session::do_closed(const augrt_object& sock) const AUG_NOTHROW
{
    scoped_frame frame(&session_);
    module_->closed(sock);
}

void
session::do_teardown(const augrt_object& sock) const AUG_NOTHROW
{
    scoped_frame frame(&session_);
    module_->teardown(sock);
}

bool
session::do_accepted(augrt_object& sock, const char* addr,
                     unsigned short port) const AUG_NOTHROW
{
    scoped_frame frame(&session_);
    return module_->accepted(sock, addr, port);
}

void
session::do_connected(augrt_object& sock, const char* addr,
                      unsigned short port) const AUG_NOTHROW
{
    scoped_frame frame(&session_);
    module_->connected(sock, addr, port);
}

void
session::do_data(const augrt_object& sock, const char* buf,
                 size_t size) const AUG_NOTHROW
{
    scoped_frame frame(&session_);
    module_->data(sock, buf, size);
}

void
session::do_rdexpire(const augrt_object& sock, unsigned& ms) const AUG_NOTHROW
{
    scoped_frame frame(&session_);
    module_->rdexpire(sock, ms);
}

void
session::do_wrexpire(const augrt_object& sock, unsigned& ms) const AUG_NOTHROW
{
    scoped_frame frame(&session_);
    module_->wrexpire(sock, ms);
}

void
session::do_expire(const augrt_object& timer, unsigned& ms) const AUG_NOTHROW
{
    scoped_frame frame(&session_);
    module_->expire(timer, ms);
}

bool
session::do_authcert(const augrt_object& sock, const char* subject,
                     const char* issuer) const AUG_NOTHROW
{
    scoped_frame frame(&session_);
    return module_->authcert(sock, subject, issuer);
}

session::~session() AUG_NOTHROW
{
    if (active_) {
        scoped_frame frame(&session_);
        module_->stop(); // AUG_NOTHROW
    }
}

session::session(const moduleptr& module, const char* name)
    : module_(module),
      active_(false)
{
    aug_strlcpy(session_.name_, name, sizeof(session_.name_));
    session_.user_ = 0;
}

const augrt_session*
augrt::getsession()
{
    return stack_.empty() ? 0 : stack_.top();
}