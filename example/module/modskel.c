#include "augas.h"

#include <stdlib.h>

static const struct augas_host* host_ = NULL;

static void
closesess_(const struct augas_sess* sess)
{
    host_->writelog_(sess->name_, AUGAS_LOGINFO, "closesess_()");
}

static int
opensess_(struct augas_sess* sess)
{
    host_->writelog_(sess->name_, AUGAS_LOGINFO, "opensess_()");
    return 0;
}

static int
event_(const struct augas_sess* sess, int type, void* user)
{
    host_->writelog_(sess->name_, AUGAS_LOGINFO, "event_()");
    return 0;
}

static int
expire_(const struct augas_sess* sess, unsigned tid, void* user,
        unsigned* ms)
{
    host_->writelog_(sess->name_, AUGAS_LOGINFO, "expire_()");
    return 0;
}

static int
reconf_(const struct augas_sess* sess)
{
    host_->writelog_(sess->name_, AUGAS_LOGINFO, "reconf_()");
    return 0;
}

static void
closeconn_(const struct augas_conn* conn)
{
    host_->writelog_(conn->sess_->name_, AUGAS_LOGINFO, "closeconn_()");
}

static int
openconn_(struct augas_conn* conn, const char* addr, unsigned short port)
{
    host_->writelog_(conn->sess_->name_, AUGAS_LOGINFO, "openconn_()");
    return 0;
}

static void
notconn_(const struct augas_conn* conn)
{
    host_->writelog_(conn->sess_->name_, AUGAS_LOGINFO, "notconn_()");
}

static int
data_(const struct augas_conn* conn, const char* buf, size_t size)
{
    host_->writelog_(conn->sess_->name_, AUGAS_LOGINFO, "data_()");
    host_->send_(conn->sess_->name_, conn->id_, buf, size, AUGAS_SNDSELF);
    return 0;
}

static int
rdexpire_(const struct augas_conn* conn, unsigned* ms)
{
    host_->writelog_(conn->sess_->name_, AUGAS_LOGINFO, "rdexpire_()");
    return 0;
}

static int
wrexpire_(const struct augas_conn* conn, unsigned* ms)
{
    host_->writelog_(conn->sess_->name_, AUGAS_LOGINFO, "wrexpire_()");
    return 0;
}

static int
teardown_(const struct augas_conn* conn)
{
    host_->writelog_(conn->sess_->name_, AUGAS_LOGINFO, "teardown_()");
    host_->shutdown_(conn->id_);
    return 0;
}

static const struct augas_module fntable_ = {
    closesess_,
    opensess_,
    event_,
    expire_,
    reconf_,
    closeconn_,
    openconn_,
    notconn_,
    data_,
    rdexpire_,
    wrexpire_,
    teardown_
};

static const struct augas_module*
load_(const char* name, const struct augas_host* host)
{
    if (host_)
        return NULL;

    host_ = host;
    return &fntable_;
}

static void
unload_(void)
{
    host_ = 0;
}

AUGAS_MODULE(load_, unload_)