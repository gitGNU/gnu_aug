/* Copyright (c) 2004-2007, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#error deprecated
#ifndef AUGSYS_SOCKET_H
#define AUGSYS_SOCKET_H

/**
 * @file augsys/socket.h
 *
 * Sockets.
 */

#include "augsys/config.h"
#include "augsys/types.h"

#if !defined(_WIN32)
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
#else /* _WIN32 */
# include <winsock2.h>
# include <ws2tcpip.h>
# if !defined(bzero)
#  define bzero ZeroMemory
# endif /* !bzero */
# if !defined(SHUT_RD)
#  define SHUT_RD SD_RECEIVE
#  define SHUT_WR SD_SEND
#  define SHUT_RDWR SD_BOTH
# endif /* !SHUT_RD */
# if !defined(socklen_t)
typedef int socklen_t;
# endif /* socklen_t */
#endif /* _WIN32 */

/**
 * Maximum sockaddr size.
 */

#define AUG_MAXADDRLEN 128

/**
 * Maximum hostname length.
 *
 * Defined as 64 on some operating systems.  Large enough to contain the
 * following formats:
 *
 * @li ipv4: 127.0.0.1
 * @li ipv6: 2001:0db8:85a3:08d3:1319:8a2e:0370:7344
 */

#define AUG_MAXHOSTNAMELEN 64

/**
 * Maximum host/port pair length.
 *
 * Large enough to contain the following formats:
 *
 * @li ipv4: 127.0.0.1:443
 * @li ipv6: [2001:0db8:85a3:08d3:1319:8a2e:0370:7344]:443
 */

#define AUG_MAXHOSTSERVLEN 128

struct aug_endpoint {
    socklen_t len_;
    union {
        short family_;
        struct {
            short family_;
            unsigned short port_;
        } all_;
        struct sockaddr sa_;
        struct sockaddr_in ipv4_;
#if HAVE_IPV6
        struct sockaddr_in6 ipv6_;
#endif /* HAVE_IPV6 */
        char pad_[AUG_MAXADDRLEN];
    } un_;
};

struct aug_inetaddr {
    short family_;
    union {
        struct in_addr ipv4_;
#if HAVE_IPV6
        struct in6_addr ipv6_;
#endif /* HAVE_IPV6 */
        char pad_[16];
    } un_;
};

AUGSYS_API aug_fd
aug_socket(aug_ctx* ctx, int domain, int type, int protocol);

AUGSYS_API aug_result
aug_accept(aug_ctx* ctx, aug_fd sd, struct aug_endpoint* ep);

AUGSYS_API aug_result
aug_bind(aug_ctx* ctx, aug_fd sd, const struct aug_endpoint* ep);

/**
 * Remember that, for non-blocking sockets, connect() may fail with
 * #EINPROGRESS.  Use poll() or select() on write-status to determine
 * completion.
 */

AUGSYS_API aug_result
aug_connect(aug_ctx* ctx, aug_fd sd, const struct aug_endpoint* ep);

AUGSYS_API struct aug_endpoint*
aug_getpeername(aug_ctx* ctx, aug_fd sd, struct aug_endpoint* ep);

AUGSYS_API struct aug_endpoint*
aug_getsockname(aug_ctx* ctx, aug_fd sd, struct aug_endpoint* ep);

AUGSYS_API aug_result
aug_listen(aug_ctx* ctx, aug_fd sd, int backlog);

AUGSYS_API ssize_t
aug_recv(aug_ctx* ctx, aug_fd sd, void* buf, size_t len, int flags);

AUGSYS_API ssize_t
aug_recvfrom(aug_ctx* ctx, aug_fd sd, void* buf, size_t len, int flags,
             struct aug_endpoint* ep);

AUGSYS_API ssize_t
aug_send(aug_ctx* ctx, aug_fd sd, const void* buf, size_t len, int flags);

AUGSYS_API ssize_t
aug_sendto(aug_ctx* ctx, aug_fd sd, const void* buf, size_t len, int flags,
           const struct aug_endpoint* ep);

AUGSYS_API aug_result
aug_getsockopt(aug_ctx* ctx, aug_fd sd, int level, int optname, void* optval,
               socklen_t* optlen);

AUGSYS_API aug_result
aug_setsockopt(aug_ctx* ctx, aug_fd sd, int level, int optname,
               const void* optval, socklen_t optlen);

/**
 * Shutdown the socket.
 *
 * Use #SHUT_WR to gracefully shutdown TCP sockets: a FIN will be sent after
 * all data is sent and acknowledged by the receiver.
 */

AUGSYS_API aug_result
aug_shutdown(aug_ctx* ctx, aug_fd sd, int how);

AUGSYS_API aug_result
aug_socketpair(aug_ctx* ctx, int domain, int type, int protocol,
               aug_fd sv[2]);

AUGSYS_API char*
aug_endpointntop(aug_ctx* ctx, const struct aug_endpoint* src, char* dst,
                 socklen_t len);

AUGSYS_API char*
aug_inetntop(aug_ctx* ctx, const struct aug_inetaddr* src, char* dst,
             socklen_t len);

AUGSYS_API struct aug_inetaddr*
aug_inetpton(aug_ctx* ctx, int af, const char* src, struct aug_inetaddr* dst);

AUGSYS_API void
aug_destroyaddrinfo(aug_ctx* ctx, struct addrinfo* res);

/**
 * Protocol independent translation of @a host and @a serv to address list.
 *
 * The resulting address list is dynamically allocated, aug_destroyaddrinfo()
 * should be used to free this list.
 *
 * @param host Host part of the address.
 * @param serv Service or port part of the address
 * @param hints Hints about the desired socket type.
 * @param res The output address list.
 *
 * @return -1 on failure.
 */

AUGSYS_API aug_result
aug_getaddrinfo(aug_ctx* ctx, const char* host, const char* serv,
                const struct addrinfo* hints, struct addrinfo** res);

AUGSYS_API int
aug_getfamily(aug_ctx* ctx, aug_fd s);

AUGSYS_API aug_result
aug_setreuseaddr(aug_ctx* ctx, aug_fd s, int on);

AUGSYS_API struct aug_endpoint*
aug_getendpoint(aug_ctx* ctx, const struct addrinfo* addr,
                struct aug_endpoint* ep);

AUGSYS_API struct aug_endpoint*
aug_setinetaddr(aug_ctx* ctx, struct aug_endpoint* ep,
                const struct aug_inetaddr* addr);

AUGSYS_API struct aug_inetaddr*
aug_getinetaddr(aug_ctx* ctx, const struct aug_endpoint* ep,
                struct aug_inetaddr* addr);

AUGSYS_API const struct aug_inetaddr*
aug_inetany(aug_ctx* ctx, int af);

AUGSYS_API const struct aug_inetaddr*
aug_inetloopback(aug_ctx* ctx, int af);

/**
 * After a failed call to aug_accept(), this function can be used to determine
 * whether the error was a result of the peer closing the connection prior to
 * aug_accept() being called.  This can occur when the passive socket is
 * non-blocking.
 */

AUGSYS_API aug_bool
aug_acceptlost(aug_ctx* ctx);

#endif /* AUGSYS_SOCKET_H */
