/* Copyright (c) 2004-2009, Mark Aylett <mark.aylett@gmail.com>
   See the file COPYING for copying permission.
*/
#ifndef AUGSYSPP_SOCKET_HPP
#define AUGSYSPP_SOCKET_HPP

#include "augsyspp/smartfd.hpp"

#include "augctxpp/exception.hpp"

#include "augsys/socket.h"

#include <string>

namespace aug {

    inline void
    close(sdref ref)
    {
        verify(aug_sclose(ref.get()));
    }

    /**
     * Set socket non-blocking on or off.
     *
     * @param ref Socket descriptor.
     *
     * @param on On or off.
     */

    inline void
    setnonblock(sdref ref, bool on)
    {
        verify(aug_ssetnonblock(ref.get(), on ? 1 : 0));
    }

    inline autosd
    socket(int domain, int type, int protocol = 0)
    {
        autosd sd(aug_socket(domain, type, protocol), close);
        if (null == sd)
            throwerror();
        return sd;
    }

    inline autosd
    accept(sdref ref, aug_endpoint& ep)
    {
        autosd sd(aug_accept(ref.get(), &ep), close);
        if (null == sd)
            throwerror();
        return sd;
    }

    inline void
    bind(sdref ref, const aug_endpoint& ep)
    {
        verify(aug_bind(ref.get(), &ep));
    }

    inline void
    connect(sdref ref, const aug_endpoint& ep)
    {
        verify(aug_connect(ref.get(), &ep));
    }

    inline aug_endpoint&
    getpeername(sdref ref, aug_endpoint& ep)
    {
        return *verify(aug_getpeername(ref.get(), &ep));
    }

    inline const aug_endpoint&
    getsockname(sdref ref, aug_endpoint& ep)
    {
        return *verify(aug_getsockname(ref.get(), &ep));
    }

    inline void
    listen(sdref ref, int backlog)
    {
        verify(aug_listen(ref.get(), backlog));
    }

    inline size_t
    recv(sdref ref, void* buf, size_t len, int flags)
    {
        return AUG_RESULT(verify(aug_recv(ref.get(), buf, len, flags)));
    }

    inline size_t
    recvfrom(sdref ref, void* buf, size_t len, int flags,
             aug_endpoint& ep)
    {
        return AUG_RESULT(verify(aug_recvfrom(ref.get(), buf, len, flags,
                                              &ep)));
    }

    inline size_t
    send(sdref ref, const void* buf, size_t len, int flags)
    {
        return AUG_RESULT(verify(aug_send(ref.get(), buf, len, flags)));
    }

    inline size_t
    sendto(sdref ref, const void* buf, size_t len, int flags,
           const aug_endpoint& ep)
    {
        return AUG_RESULT(verify(aug_sendto(ref.get(), buf, len, flags,
                                            &ep)));
    }

    inline size_t
    read(sdref ref, void* buf, size_t len)
    {
        return AUG_RESULT(verify(aug_sread(ref.get(), buf, len)));
    }

    inline size_t
    readv(sdref ref, const struct iovec* iov, int size)
    {
        return AUG_RESULT(verify(aug_sreadv(ref.get(), iov, size)));
    }

    inline size_t
    write(sdref ref, const void* buf, size_t len)
    {
        return AUG_RESULT(verify(aug_swrite(ref.get(), buf, len)));
    }

    inline size_t
    writev(sdref ref, const struct iovec* iov, int size)
    {
        return AUG_RESULT(verify(aug_swritev(ref.get(), iov, size)));
    }

    inline void
    getsockopt(sdref ref, int level, int optname, void* optval,
               socklen_t& optlen)
    {
        verify(aug_getsockopt(ref.get(), level, optname, optval, &optlen));
    }

    inline void
    setsockopt(sdref ref, int level, int optname, const void* optval,
               socklen_t optlen)
    {
        verify(aug_setsockopt(ref.get(), level, optname, optval, optlen));
    }

    inline void
    shutdown(sdref ref, int how)
    {
        verify(aug_sshutdown(ref.get(), how));
    }

    inline autosds
    socketpair(int domain, int type, int protocol)
    {
        aug_sd sv[2];
        verify(aug_socketpair(domain, type, protocol, sv));
        return autosds(sv[0], sv[1], close);
    }

    inline std::string
    endpointntop(const aug_endpoint& src)
    {
        char buf[AUG_MAXHOSTSERVLEN + 1];
        verify(aug_endpointntop(&src, buf, sizeof(buf)));
        return buf;
    }

    inline std::string
    inetntop(const aug_inetaddr& src)
    {
        char buf[AUG_MAXHOSTNAMELEN + 1];
        verify(aug_inetntop(&src, buf, sizeof(buf)));
        return buf;
    }

    inline aug_inetaddr&
    inetpton(int af, const char* src, aug_inetaddr& dst)
    {
        return *verify(aug_inetpton(af, src, &dst));
    }

    inline aug_inetaddr&
    inetpton(const char* src, aug_inetaddr& dst)
    {
        if (!aug_inetpton(AF_INET, src, &dst)
            && !aug_inetpton(AF_INET6, src, &dst))
            throwerror();
        return dst;
    }

    inline int
    getfamily(sdref ref)
    {
        return AUG_RESULT(verify(aug_getfamily(ref.get())));
    }

    inline void
    setreuseaddr(sdref ref, bool on)
    {
        verify(aug_setreuseaddr(ref.get(), on ? AUG_TRUE : AUG_FALSE));
    }

    inline aug_endpoint&
    getendpoint(const addrinfo& addr, aug_endpoint& ep)
    {
        return *verify(aug_getendpoint(&addr, &ep));
    }

    inline aug_endpoint&
    setinetaddr(aug_endpoint& ep, const aug_inetaddr& addr)
    {
        return *verify(aug_setinetaddr(&ep, &addr));
    }

    inline aug_inetaddr&
    getinetaddr(const aug_endpoint& ep, aug_inetaddr& addr)
    {
        return *verify(aug_getinetaddr(&ep, &addr));
    }

    inline const aug_inetaddr&
    inetany(int af)
    {
        return *verify(aug_inetany(af));
    }

    inline const aug_inetaddr&
    inetloopback(int af)
    {
        return *verify(aug_inetloopback(af));
    }
}

#endif // AUGSYSPP_SOCKET_HPP
