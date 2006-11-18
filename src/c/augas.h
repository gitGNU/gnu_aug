/* Copyright (c) 2004-2006, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#ifndef AUGAS_H
#define AUGAS_H

#include <stdarg.h>    /* va_list */
#include <sys/types.h> /* size_t */

#if !defined(__cplusplus)
# define AUGAS_EXTERN extern
#else /* __cplusplus */
# define AUGAS_EXTERN extern "C"
#endif /* __cplusplus */

#if !defined(_WIN32)
# define AUGAS_EXPORT AUGAS_EXTERN
# define AUGAS_IMPORT AUGAS_EXTERN
#else /* _WIN32 */
# define AUGAS_EXPORT AUGAS_EXTERN __declspec(dllexport)
# define AUGAS_IMPORT AUGAS_EXTERN __declspec(dllimport)
#endif /* _WIN32 */

#if !defined(AUGAS_BUILD)
# define AUGAS_API AUGAS_EXPORT
#else /* AUGAS_BUILD */
# define AUGAS_API AUGAS_IMPORT
#endif /* AUGAS_BUILD */

enum augas_loglevel {
    AUGAS_LOGCRIT,
    AUGAS_LOGERROR,
    AUGAS_LOGWARN,
    AUGAS_LOGNOTICE,
    AUGAS_LOGINFO,
    AUGAS_LOGDEBUG
};

#define AUGAS_TIMRD     0x01
#define AUGAS_TIMWR     0x02
#define AUGAS_TIMBOTH  (AUGAS_TIMRD | AUGAS_TIMWR)

#define AUGAS_SNDSELF   0x01
#define AUGAS_SNDOTHER  0x02
#define AUGAS_SNDALL   (AUGAS_SNDSELF | AUGAS_SNDOTHER)

#define AUGAS_SUCCESS   0
#define AUGAS_FAILURE (-1)

#define AUGAS_MAXNAME   63

/**
   sessions = foo bar
   session.foo.module = modpython
   session.bar.module = modpython
   module.modpython.path = modpython.dll
*/

typedef unsigned augas_id;

struct augas_sess {
    char name_[AUGAS_MAXNAME + 1];
    void* user_;
};

struct augas_conn {
    const struct augas_sess* sess_;
    augas_id id_;
    void* user_;
};

struct augas_host {

    /**
       \return the last error that occurred.
    */

    const char* (*error_)(void);
    void (*writelog_)(int level, const char* format, ...);
    void (*vwritelog_)(int level, const char* format, va_list args);

    /**
       \return the value associated with name.
    */

    const char* (*getenv_)(const char* sname, const char* name);

    /**
       \return the new conn id.
    */

    int (*tcpconnect_)(const char* sname, const char* host, const char* serv,
                       void* user);
    int (*tcplisten_)(const char* sname, const char* host, const char* serv,
                      void* user);
    int (*post_)(const char* sname, int type, void* user);
    int (*settimer_)(const char* sname, int tid, unsigned ms, void* user);
    int (*resettimer_)(const char* sname, int tid, unsigned ms);
    int (*canceltimer_)(const char* sname, int tid);
    int (*shutdown_)(augas_id cid);
    int (*send_)(const char* sname, augas_id cid, const char* buf,
                 size_t size, unsigned flags);
    int (*setrwtimer_)(augas_id cid, unsigned ms, unsigned flags);
    int (*resetrwtimer_)(augas_id cid, unsigned ms, unsigned flags);
    int (*cancelrwtimer_)(augas_id cid, unsigned flags);
};

/**
   Module functions should return either #AUGAS_SUCCESS or #AUGAS_FAILURE.
   For those functions associated with conns, a failure will result in the
   conn being closed.
*/

struct augas_module {

    void (*closesess_)(const struct augas_sess* sess);
    int (*opensess_)(struct augas_sess* sess);
    int (*event_)(const struct augas_sess* sess, int type, void* user);
    int (*expire_)(const struct augas_sess* sess, unsigned tid, void* user,
                   unsigned* ms);
    int (*reconf_)(const struct augas_sess* sess);

    void (*closeconn_)(const struct augas_conn* conn);
    int (*openconn_)(struct augas_conn* conn, const char* addr,
                     unsigned short port);

    /**
       Notifies the application that a previous call to tcpconnect_() has
       failed.
    */

    void (*notconn_)(const struct augas_conn* conn);
    int (*data_)(const struct augas_conn* conn, const char* buf, size_t size);
    int (*rdexpire_)(const struct augas_conn* conn, unsigned* ms);
    int (*wrexpire_)(const struct augas_conn* conn, unsigned* ms);
    int (*teardown_)(const struct augas_conn* conn);
};

/**
   augas_load() should return NULL on failure.
*/

#define AUGAS_MODULE(init, term)                                    \
    AUGAS_API void                                                  \
    augas_term(void)                                                \
    {                                                               \
        (*term)();                                                  \
    }                                                               \
    AUGAS_API const struct augas_module*                            \
    augas_init(const char* name, const struct augas_host* host)     \
    {                                                               \
        return (*init)(name, host);                                 \
    }

typedef void (*augas_termfn)(void);
typedef const struct augas_module* (*augas_initfn)(const char*,
                                                   const struct augas_host*);

#endif /* AUGAS_H */
