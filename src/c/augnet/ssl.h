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
#ifndef AUGNET_SSL_H
#define AUGNET_SSL_H

/**
 * @file augnet/ssl.h
 *
 * SSL support.
 */

#include "augnet/config.h"

#include "augsys/muxer.h"

#include "augext/chan.h"
#include "augext/mpool.h"

struct aug_errinfo;
struct ssl_ctx_st;

struct aug_ssldata {
    aug_chandler* handler_;
    aug_id id_;
};

AUGNET_API void
aug_setsslerrinfo(struct aug_errinfo* errinfo, const char* file, int line,
                  unsigned long err);

/**
 * Create an ssl client channel.
 *
 * Must be passed a non-blocking socket.  If successful, will assume
 * responsibility for calling aug_sclose() on socket.
 *
 * Assumes that @ref aug_chandler has already been notified of connection
 * establishment for @a id.
 */

AUGNET_API aug_chan*
aug_createsslclient(aug_mpool* mpool, aug_id id, aug_muxer_t muxer,
                    aug_sd sd, aug_bool wantwr, aug_chandler* handler,
                    struct ssl_ctx_st* sslctx);

/**
 * Create an ssl server channel.
 *
 * Must be passed a non-blocking socket.  If successful, will assume
 * responsibility for calling aug_sclose() on socket.
 *
 * Assumes that @ref aug_chandler has already been notified of connection
 * establishment for @a id.
 */

AUGNET_API aug_chan*
aug_createsslserver(aug_mpool* mpool, aug_id id, aug_muxer_t muxer,
                    aug_sd sd, aug_bool wantwr, aug_chandler* handler,
                    struct ssl_ctx_st* sslctx);

#endif /* AUGNET_SSL_H */
