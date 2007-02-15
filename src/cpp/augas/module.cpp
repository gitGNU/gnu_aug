/* Copyright (c) 2004-2007, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#define AUGAS_BUILD
#include "augas/module.hpp"

#include "augas/exception.hpp"
#include "augas/utility.hpp"

#include "augsys/log.h"

#include <exception>

using namespace aug;
using namespace augas;
using namespace std;

module::~module() AUG_NOTHROW
{
    try {
        AUG_DEBUG2("terminating module: name=[%s]", name_.c_str());
        termfn_();
    } AUG_PERRINFOCATCH;
}

module::module(const string& name, const char* path,
               const struct augas_host& host)
    : name_(name),
      lib_(path)
{
    AUG_DEBUG2("resolving symbols in module: name=[%s]", name_.c_str());
    augas_initfn initfn(dlsym<augas_initfn>(lib_, "augas_init"));
    termfn_ = dlsym<augas_termfn>(lib_, "augas_term");

    AUG_DEBUG2("initialising module: name=[%s]", name_.c_str());
    const struct augas_module* ptr(initfn(name_.c_str(), &host));
    if (!ptr)
        throw error(__FILE__, __LINE__, EMODCALL, "augas_init() failed");
    setdefaults(module_, *ptr);
}

void
module::destroy(const augas_serv& serv) const AUG_NOTHROW
{
    AUG_DEBUG2("destroy(): sname=[%s]", serv.name_);
    module_.destroy_(&serv);
}

bool
module::create(augas_serv& serv) const AUG_NOTHROW
{
    AUG_DEBUG2("create(): sname=[%s]", serv.name_);
    return AUGAS_OK == module_.create_(&serv);
}

bool
module::reconf(const augas_serv& serv) const AUG_NOTHROW
{
    AUG_DEBUG2("reconf(): sname=[%s]", serv.name_);
    return AUGAS_OK == module_.reconf_(&serv);
}

bool
module::event(const augas_serv& serv, const char* from,
              const augas_event& event) const AUG_NOTHROW
{
    AUG_DEBUG2("event(): sname=[%s], from=[%s], ename=[%s], size=[%d]",
               serv.name_, from, event.type_, event.size_);
    return AUGAS_OK == module_.event_(&serv, from, &event);
}

void
module::closed(const augas_object& sock) const AUG_NOTHROW
{
    AUG_DEBUG2("closed(): sname=[%s], id=[%d]", sock.serv_->name_, sock.id_);
    module_.closed_(&sock);
}

bool
module::teardown(const augas_object& sock) const AUG_NOTHROW
{
    AUG_DEBUG2("teardown(): sname=[%s], id=[%d]",
               sock.serv_->name_, sock.id_);
    return AUGAS_OK == module_.teardown_(&sock);
}

bool
module::accept(augas_object& sock, const char* addr,
               unsigned short port) const AUG_NOTHROW
{
    AUG_DEBUG2("accept(): sname=[%s], id=[%d], addr=[%s], port=[%u]",
               sock.serv_->name_, sock.id_, addr, (unsigned)port);
    return AUGAS_OK == module_.accept_(&sock, addr, port);
}

bool
module::connected(augas_object& sock, const char* addr,
                unsigned short port) const AUG_NOTHROW
{
    AUG_DEBUG2("connected(): sname=[%s], id=[%d], addr=[%s], port=[%u]",
               sock.serv_->name_, sock.id_, addr, (unsigned)port);
    return AUGAS_OK == module_.connected_(&sock, addr, port);
}

bool
module::data(const augas_object& sock, const char* buf,
             size_t size) const AUG_NOTHROW
{
    AUG_DEBUG2("data(): sname=[%s], id=[%d]", sock.serv_->name_, sock.id_);
    return AUGAS_OK == module_.data_(&sock, buf, size);
}

bool
module::rdexpire(const augas_object& sock, unsigned& ms) const AUG_NOTHROW
{
    AUG_DEBUG2("rdexpire(): sname=[%s], id=[%d], ms=[%u]",
               sock.serv_->name_, sock.id_, ms);
    return AUGAS_OK == module_.rdexpire_(&sock, &ms);
}

bool
module::wrexpire(const augas_object& sock, unsigned& ms) const AUG_NOTHROW
{
    AUG_DEBUG2("wrexpire(): sname=[%s], id=[%d], ms=[%u]",
               sock.serv_->name_, sock.id_, ms);
    return AUGAS_OK == module_.wrexpire_(&sock, &ms);
}

bool
module::expire(const augas_object& timer, unsigned& ms) const AUG_NOTHROW
{
    AUG_DEBUG2("expire(): sname=[%s], id=[%d], ms=[%u]",
               timer.serv_->name_, timer.id_, ms);
    return AUGAS_OK == module_.expire_(&timer, &ms);
}
