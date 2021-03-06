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

#include "augctx/base.h"
#include "augctx/errinfo.h"

#if defined(__GNUC__)
# include "iptypes.h"
#endif /* __GNUC__ */
#include <iphlpapi.h>

static aug_result
findif_(int af, unsigned ifindex,
        aug_result (*fn)(void*, unsigned, PIP_ADAPTER_ADDRESSES), void* arg)
{
    aug_mpool* mpool = aug_getmpool(aug_tlx);
    int i;
    PIP_ADAPTER_ADDRESSES list = NULL;
    ULONG len = 0, ret = 0;
    aug_result result;

    /* The size of the buffer can be different between consecutive API calls.
       In most cases, i < 2 is sufficient; one call to get the size and one
       call to get the actual parameters.  But if one more interface is added
       or addresses are added, the call again fails with BUFFER_OVERFLOW.  So
       the number is picked slightly greater than 2. */

    for (i = 0; i < 5; i++) {

        /* First call with null list to get size. */

        ret = GetAdaptersAddresses(af, 0, NULL, list, &len);
        if (ERROR_BUFFER_OVERFLOW != ret)
            break;

        if (list)
            aug_freemem(mpool, list);

        if (!(list = aug_allocmem(mpool, len))) {
            aug_release(mpool);
            return -1;
        }
    }

    if (NO_ERROR == ret) {

        PIP_ADAPTER_ADDRESSES it = list;
        for (i = 1; it; ++i, it = it->Next)
            if (i == (int)ifindex)
                break;

        if (it)
            result = fn(arg, ifindex, it);
        else {
            aug_setctxerror(aug_tlx, __FILE__, __LINE__, "aug", AUG_EEXIST,
                            AUG_MSG("interface [%d] does not exist"),
                            (int)ifindex);
            result = -1;
        }

    } else {
        aug_setwin32error(aug_tlx, __FILE__, __LINE__, ret);
        result = -1;
    }

    if (list)
        aug_freemem(mpool, list);
    aug_release(mpool);

    return result;
}

static aug_result
ifaddr_(void* arg, unsigned ifindex, PIP_ADAPTER_ADDRESSES adapter)
{
    struct in_addr* out = arg;
    PIP_ADAPTER_UNICAST_ADDRESS it = adapter->FirstUnicastAddress;

    /* Iteration is not strictly necessary: the addresses should already be
       filtered by AF_INET. */

    for (; it ; it = it->Next)
        if (AF_INET == it->Address.lpSockaddr->sa_family) {
            const struct sockaddr_in* addr = (const struct sockaddr_in*)it
                ->Address.lpSockaddr;
            out->s_addr = addr->sin_addr.s_addr;
            return 0;
        }

    aug_setctxerror(aug_tlx, __FILE__, __LINE__, "aug", AUG_EEXIST,
                    AUG_MSG("no address for interface [%d]"), (int)ifindex);
    return -1;
}

#if HAVE_IPV6
static aug_result
ifindex_(void* arg, unsigned ifindex, PIP_ADAPTER_ADDRESSES adapter)
{
    DWORD* out = arg;
    PIP_ADAPTER_UNICAST_ADDRESS it = adapter->FirstUnicastAddress;

    /* Iteration is not strictly necessary: the addresses should already be
       filtered by AF_INET6. */

    for (; it ; it = it->Next)
        if (AF_INET6 == it->Address.lpSockaddr->sa_family) {
            *out = adapter->Ipv6IfIndex;
            return 0;
        }

    aug_setctxerror(aug_tlx, __FILE__, __LINE__, "aug", AUG_EEXIST,
                    AUG_MSG("no address for interface [%d]"), (int)ifindex);
    return -1;
}
#endif /* HAVE_IPV6 */

static aug_result
getifaddr_(struct in_addr* addr, const char* ifname)
{
    unsigned ifindex = atoi(ifname);
    if (0 == ifindex) {
        aug_setctxerror(aug_tlx, __FILE__, __LINE__, "aug", AUG_EINVAL,
                        AUG_MSG("invalid interface index [%s]"), ifname);
        return -1;
    }
    return findif_(AF_INET, ifindex, ifaddr_, addr);
}

#if HAVE_IPV6
static aug_result
getifindex_(DWORD* index, const char* ifname)
{
    unsigned ifindex = atoi(ifname);
    if (0 == ifindex) {
        aug_setctxerror(aug_tlx, __FILE__, __LINE__, "aug", AUG_EINVAL,
                        AUG_MSG("invalid interface index [%s]"), ifname);
        return -1;
    }
    return findif_(AF_INET6, ifindex, ifindex_, index);
}
#endif /* HAVE_IPV6 */

AUGSYS_API aug_result
aug_joinmcast(aug_sd sd, const struct aug_inetaddr* addr, const char* ifname)
{
    union {
        struct ip_mreq ipv4_;
#if HAVE_IPV6
        struct ipv6_mreq ipv6_;
#endif /* HAVE_IPV6 */
    } un;

    switch (addr->family_) {
    case AF_INET:

        un.ipv4_.imr_multiaddr.s_addr = addr->un_.ipv4_.s_addr;

        if (ifname) {

            struct in_addr ifaddr;
            if (getifaddr_(&ifaddr, ifname) < 0)
                return -1;

            un.ipv4_.imr_interface.s_addr = ifaddr.s_addr;
        } else
            un.ipv4_.imr_interface.s_addr = htonl(INADDR_ANY);

        return aug_setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &un.ipv4_,
                              sizeof(un.ipv4_));
#if HAVE_IPV6
    case AF_INET6:

		memcpy(&un.ipv6_.ipv6mr_multiaddr, &addr->un_.ipv6_,
			   sizeof(addr->un_.ipv6_));

        if (ifname) {

            DWORD i;
            if (getifindex_(&i, ifname) < 0)
                return -1;

			un.ipv6_.ipv6mr_interface = i;
        } else
			un.ipv6_.ipv6mr_interface = 0;

        return aug_setsockopt(sd, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP,
                              &un.ipv6_, sizeof(un.ipv6_));
#endif /* HAVE_IPV6 */
    }

    aug_setwin32error(aug_tlx, __FILE__, __LINE__, WSAEAFNOSUPPORT);
    return -1;
}

AUGSYS_API aug_result
aug_leavemcast(aug_sd sd, const struct aug_inetaddr* addr, const char* ifname)
{
    union {
        struct ip_mreq ipv4_;
#if HAVE_IPV6
        struct ipv6_mreq ipv6_;
#endif /* HAVE_IPV6 */
    } un;

    switch (addr->family_) {
    case AF_INET:

        un.ipv4_.imr_multiaddr.s_addr = addr->un_.ipv4_.s_addr;

        if (ifname) {

            struct in_addr ifaddr;
            if (getifaddr_(&ifaddr, ifname) < 0)
                return -1;

            un.ipv4_.imr_interface.s_addr = ifaddr.s_addr;
        } else
            un.ipv4_.imr_interface.s_addr = htonl(INADDR_ANY);

        return aug_setsockopt(sd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &un.ipv4_,
                              sizeof(un.ipv4_));
#if HAVE_IPV6
    case AF_INET6:

		memcpy(&un.ipv6_.ipv6mr_multiaddr, &addr->un_.ipv6_,
			   sizeof(addr->un_.ipv6_));

        if (ifname) {

            DWORD i;
            if (getifindex_(&i, ifname) < 0)
                return -1;

			un.ipv6_.ipv6mr_interface = i;
        } else
			un.ipv6_.ipv6mr_interface = 0;

        return aug_setsockopt(sd, IPPROTO_IPV6, IPV6_DROP_MEMBERSHIP,
                              &un.ipv6_, sizeof(un.ipv6_));
#endif /* HAVE_IPV6 */
    }

    aug_setwin32error(aug_tlx, __FILE__, __LINE__, WSAEAFNOSUPPORT);
    return -1;
}

AUGSYS_API aug_result
aug_setmcastif(aug_sd sd, const char* ifname)
{
    aug_rint af;
    union {
        struct in_addr ipv4_;
#if HAVE_IPV6
        DWORD ipv6_;
#endif /* HAVE_IPV6 */
    } un;

    if ((af = aug_getfamily(sd)) < 0)
        return -1;

    switch (af) {
    case AF_INET:

        if (getifaddr_(&un.ipv4_, ifname) < 0)
            return -1;
        return aug_setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, &un.ipv4_,
                              sizeof(un.ipv4_));
#if HAVE_IPV6
    case AF_INET6:

        if (getifindex_(&un.ipv6_, ifname) < 0)
            return -1;
        return aug_setsockopt(sd, IPPROTO_IPV6, IPV6_MULTICAST_IF,
                              &un.ipv6_, sizeof(un.ipv6_));
#endif /* HAVE_IPV6 */
    }

    aug_setwin32error(aug_tlx, __FILE__, __LINE__, WSAEAFNOSUPPORT);
    return -1;
}

AUGSYS_API aug_result
aug_setmcastloop(aug_sd sd, aug_bool on)
{
    int opt = on ? 1 : 0;
    aug_rint af = aug_getfamily(sd);
    if (af < 0)
        return -1;

    /* On Windows, both the IPV4 and IPV6 options expect a DWORD. */

    switch (af) {
    case AF_INET:
        return aug_setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, &opt,
                              sizeof(opt));
#if HAVE_IPV6
    case AF_INET6:
        return aug_setsockopt(sd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP,
                              &opt, sizeof(opt));
#endif /* HAVE_IPV6 */
    }

    aug_setwin32error(aug_tlx, __FILE__, __LINE__, WSAEAFNOSUPPORT);
    return -1;
}

AUGSYS_API aug_result
aug_setmcastttl(aug_sd sd, int ttl)
{
    aug_rint af = aug_getfamily(sd);
    if (af < 0)
        return -1;

    /* On Windows, both the IPV4 and IPV6 options expect a DWORD. */

    switch (af) {
    case AF_INET:
        return aug_setsockopt(sd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl,
                              sizeof(ttl));
#if HAVE_IPV6
    case AF_INET6:
        return aug_setsockopt(sd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS,
                              &ttl, sizeof(ttl));
#endif /* HAVE_IPV6 */
    }

    aug_setwin32error(aug_tlx, __FILE__, __LINE__, WSAEAFNOSUPPORT);
    return -1;
}
