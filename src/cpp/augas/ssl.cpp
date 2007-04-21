/* Copyright (c) 2004-2007, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#define AUGAS_BUILD
#include "augas/ssl.hpp"
#include "augsys/defs.h"

AUG_RCSID("$Id$");

#if HAVE_OPENSSL_SSL_H

#include "augsys/base.h"

# include <openssl/err.h>

using namespace aug;
using namespace augas;
using namespace std;

namespace {

    int
    passwdcb_(char* buf, int size, int rwflag, void* password)
    {
        strncpy(buf, static_cast<const char*>(password), size);
        buf[size - 1] = '\0';
        return static_cast<int>(strlen(buf));
    }

    SSL_CTX*
    createctx_(const char* keyfile, const char* password, const char* cafile)
    {
        SSL_METHOD* meth;
        SSL_CTX* ctx;

        // Global system initialization.

        SSL_library_init();
        SSL_load_error_strings();

        // Create our context.

        meth = SSLv23_method();
        ctx = SSL_CTX_new(meth);

        // Load our keys and certificates.

        if (!(SSL_CTX_use_certificate_chain_file(ctx, keyfile)))
            throw ssl_error(__FILE__, __LINE__, ERR_get_error());

        SSL_CTX_set_default_passwd_cb(ctx, passwdcb_);
        SSL_CTX_set_default_passwd_cb_userdata
            (ctx, const_cast<char*>(password));

        if (!(SSL_CTX_use_PrivateKey_file(ctx, keyfile, SSL_FILETYPE_PEM)))
            throw ssl_error(__FILE__, __LINE__, ERR_get_error());

        // Load the CAs we trust.

        if (!(SSL_CTX_load_verify_locations(ctx, cafile, 0)))
            throw ssl_error(__FILE__, __LINE__, ERR_get_error());

#if OPENSSL_VERSION_NUMBER < 0x00905100L
        SSL_CTX_set_verify_depth(ctx, 1);
#endif
        return ctx;
    }
}

sslctx::~sslctx() AUG_NOTHROW
{
    if (ctx_)
        SSL_CTX_free(ctx_);
}

sslctx::sslctx(const char* keyfile, const char* password, const char* cafile)
    : ctx_(createctx_(keyfile, password, cafile))
{
}

void
sslctx::setsslclient(fdref ref, int flags)
{
    SSL* ssl = SSL_new(ctx_);
    BIO* sbio = BIO_new_socket(aug_getosfd(ref.get()), BIO_NOCLOSE);
    SSL_set_bio(ssl, sbio, sbio);
    SSL_set_verify(ssl, SSL_VERIFY_PEER, SSL_get_verify_callback(ssl));
    aug_setsslclient(ref.get(), ssl);
}

void
sslctx::setsslserver(fdref ref, int flags)
{
    SSL* ssl = SSL_new(ctx_);
    BIO* sbio = BIO_new_socket(aug_getosfd(ref.get()), BIO_NOCLOSE);
    SSL_set_bio(ssl, sbio, sbio);
    SSL_set_verify(ssl, SSL_VERIFY_PEER, SSL_get_verify_callback(ssl));
    aug_setsslserver(ref.get(), ssl);
}

#endif // HAVE_OPENSSL_SSL_H
