/* Copyright (c) 2004-2007, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#define MOD_BUILD
#include "augctx/defs.h"

AUG_RCSID("$Id$");

#include "augpy/host.h"
#include "augpy/object.h"

#if defined(_WIN32)
# include <direct.h>
#endif /* _WIN32 */

struct import_ {
    PyObject* module_;
    PyObject* stop_;
    PyObject* start_;
    PyObject* reconf_;
    PyObject* event_;
    PyObject* closed_;
    PyObject* teardown_;
    PyObject* accepted_;
    PyObject* connected_;
    PyObject* data_;
    PyObject* rdexpire_;
    PyObject* wrexpire_;
    PyObject* expire_;
    PyObject* authcert_;
    int open_;
};

static PyObject* augpy_ = NULL;
static PyTypeObject* type_ = NULL;

static void
setpath_(void)
{
    const char* s;
    char prev[1024];
    PyObject* sys;

    /* Store working directory for later restoration. */

    getcwd(prev, sizeof(prev));

    /* Path may be relative to run directory. */

    if ((s = mod_getenv("rundir", NULL)))
        chdir(s);

    s = mod_getenv("module.augpy.pythonpath", "python");
    mod_writelog(MOD_LOGDEBUG, "module.augpy.pythonpath=[%s]", s);
    chdir(s);

    if ((sys = PyImport_ImportModule("sys"))) {

        PyObject* path = PyObject_GetAttrString(sys, "path");
        if (path) {

            char buf[1024];
            PyObject* dir;

            getcwd(buf, sizeof(buf));

            if ((dir = PyString_FromString(buf))) {

                mod_writelog(MOD_LOGDEBUG, "adding to sys.path: %s", buf);

                PyList_Append(path, dir);

                Py_DECREF(dir);
            }

            Py_DECREF(path);
        }

        Py_DECREF(sys);
    }

    /* Restore previous working directory. */

    chdir(prev);
}

static void
printerr_(void)
{
    PyObject* type, * value, * traceback;
    PyObject* module;

    if (!PyErr_Occurred())
        return;

    /* Returns owned references. */

    PyErr_Fetch(&type, &value, &traceback);
    if ((module = PyImport_ImportModule("traceback"))) {

        PyObject* list, * empty, * message;
        list = PyObject_CallMethod(module, "format_exception", "OOO", type,
                                   value == NULL ? Py_None : value,
                                   traceback == NULL ? Py_None : traceback);
        empty = PyString_FromString("");
        message = PyObject_CallMethod(empty, "join", "O", list);

        mod_writelog(MOD_LOGERROR, "%s", PyString_AsString(message));

        Py_DECREF(message);
        Py_DECREF(empty);
        Py_DECREF(list);

        Py_DECREF(module);

    } else
        PyErr_Print();

    Py_DECREF(type);
    Py_XDECREF(value);
    Py_XDECREF(traceback);

    PyErr_Clear();
}

static PyObject*
getmethod_(PyObject* module, const char* name)
{
    PyObject* x = PyObject_GetAttrString(module, (char*)name);
    if (x) {
        if (!PyCallable_Check(x)) {
            Py_DECREF(x);
            x = NULL;
        }
    } else {
        mod_writelog(MOD_LOGDEBUG, "no binding for %s()", name);
        PyErr_Clear();
    }
    return x;
}

static void
destroyimport_(struct import_* import)
{
    Py_XDECREF(import->authcert_);
    Py_XDECREF(import->expire_);
    Py_XDECREF(import->wrexpire_);
    Py_XDECREF(import->rdexpire_);
    Py_XDECREF(import->data_);
    Py_XDECREF(import->connected_);
    Py_XDECREF(import->accepted_);
    Py_XDECREF(import->teardown_);
    Py_XDECREF(import->closed_);
    Py_XDECREF(import->event_);
    Py_XDECREF(import->reconf_);
    Py_XDECREF(import->start_);
    Py_XDECREF(import->stop_);

    Py_XDECREF(import->module_);
    free(import);
}

static struct import_*
createimport_(const char* sname)
{
    struct import_* import = calloc(1, sizeof(struct import_));
    if (!import)
        return NULL;

    if (!(import->module_ = PyImport_ImportModule((char*)sname))) {
        printerr_();
        goto fail;
    }

    import->stop_ = getmethod_(import->module_, "stop");
    import->start_ = getmethod_(import->module_, "start");
    import->reconf_ = getmethod_(import->module_, "reconf");
    import->event_ = getmethod_(import->module_, "event");
    import->closed_ = getmethod_(import->module_, "closed");
    import->teardown_ = getmethod_(import->module_, "teardown");
    import->accepted_ = getmethod_(import->module_, "accepted");
    import->connected_ = getmethod_(import->module_, "connected");
    import->data_ = getmethod_(import->module_, "data");
    import->rdexpire_ = getmethod_(import->module_, "rdexpire");
    import->wrexpire_ = getmethod_(import->module_, "wrexpire");
    import->expire_ = getmethod_(import->module_, "expire");
    import->authcert_ = getmethod_(import->module_, "authcert");
    import->open_ = 0;

    return import;

 fail:
    destroyimport_(import);
    return NULL;
}

static void
termpy_(void)
{
    int level, objects;

    if (!Py_IsInitialized())
        return;

    mod_writelog(MOD_LOGDEBUG, "finalising python interpreter");
    Py_Finalize();

    objects = augpy_handles();
    level = objects ? MOD_LOGERROR : MOD_LOGINFO;
    mod_writelog(level, "allocated objects: %d", objects);
}

static int
initpy_(void)
{
    mod_writelog(MOD_LOGDEBUG, "initialising python interpreter");
    Py_Initialize();
    /* Py_VerboseFlag = 1; */
    setpath_();

    mod_writelog(MOD_LOGDEBUG, "initialising aug module");
    if (!(type_ = augpy_createtype()))
        goto fail;

    if (!(augpy_ = augpy_createhost(type_)))
        goto fail;
    return 0;

 fail:
    termpy_();
    return -1;
}

static void
stop_(void)
{
    struct import_* import = mod_getsession()->user_;
    assert(import);

    if (import->open_ && import->stop_) {

        PyObject* x = PyObject_CallFunction(import->stop_, NULL);
        if (x) {
            Py_DECREF(x);
        } else
            printerr_();
    }

    destroyimport_(import);
}

static int
start_(struct mod_session* session)
{
    struct import_* import;
    if (!(import = createimport_(session->name_)))
        return -1;

    session->user_ = import;

    if (import->start_) {

        PyObject* x = PyObject_CallFunction(import->start_, "s",
                                            session->name_);
        if (!x) {
            printerr_();
            destroyimport_(import);
            return -1;
        } else if (x == Py_False) {
            destroyimport_(import);
            return -1;
        }
        Py_DECREF(x);
    }

    import->open_ = 1;
    return 0;
}

static void
reconf_(void)
{
    struct import_* import = mod_getsession()->user_;
    assert(import);

    if (import->reconf_) {

        PyObject* x = PyObject_CallFunction(import->reconf_, NULL);
        if (x) {
            Py_DECREF(x);
        } else
            printerr_();
    }
}

static void
event_(const char* from, const char* type, aug_object* ob)
{
    struct import_* import = mod_getsession()->user_;
    assert(import);

    if (import->event_) {

        PyObject* y;
        if (ob) {

            /* Preference is augpy_blob type. */

            PyObject* x = augpy_getblob(ob);
            if (x) {

                y = PyObject_CallFunction(import->event_, "ssO", from, type,
                                          x);
                Py_DECREF(x);
                goto done;

            } else {

                /* Fallback to aug_blob type. */

                aug_blob* blob = aug_cast(ob, aug_blobid);
                if (blob) {

                    size_t size;
                    const void* data = aug_getblobdata(blob, &size);

                    /* Unsafe to release here. */

                    if (data) {

                        /* Blob data obtained. */

                        y = PyObject_CallFunction(import->event_, "ssz#",
                                                  from, type,
                                                  (const char*)data, size);
                        aug_release(blob);
                        goto done;
                    }
                    aug_release(blob);
                }
            }
        }

        /* Null or unsupported object type. */

        y = PyObject_CallFunction(import->event_, "ssO", from, type,
                                  Py_None);

    done:
        if (y) {
            Py_DECREF(y);
        } else
            printerr_();
    }

    /* x will be Py_DECREF()-ed by destroy_(). */
}

static void
closed_(const struct mod_handle* sock)
{
    struct import_* import = mod_getsession()->user_;
    PyObject* x = sock->user_;
    assert(import);
    assert(x);

    if (import->closed_) {

        PyObject* y = PyObject_CallFunction(import->closed_, "O", x);
        if (y) {
            Py_DECREF(y);
        } else
            printerr_();
    }

    Py_DECREF(x);
}

static void
teardown_(const struct mod_handle* sock)
{
    struct import_* import = mod_getsession()->user_;
    assert(import);
    assert(sock->user_);

    if (import->teardown_) {

        PyObject* x = sock->user_;
        PyObject* y = PyObject_CallFunction(import->teardown_, "O", x);

        if (y) {
            Py_DECREF(y);
        } else
            printerr_();

    } else
        mod_shutdown(sock->id_, 0);
}

static int
accepted_(struct mod_handle* sock, const char* name)
{
    struct import_* import = mod_getsession()->user_;
    PyObject* x, * y;
    int ret = 0;
    assert(import);
    assert(sock->user_);

    x = augpy_getuser(sock->user_);
    y = augpy_createhandle(type_, sock->id_, x);
    Py_DECREF(x);

    if (!y) {

        /* closed() will not be called if accepted() fails. */

        printerr_();
        ret = -1;

    } else if (import->accepted_) {

        PyObject* z = PyObject_CallFunction(import->accepted_, "Os", y, name);

        if (!z) {

            /* closed() will not be called if accepted() fails. */

            printerr_();
            Py_DECREF(y);
            return -1;
        }

        if (z == Py_False) {

            mod_writelog(MOD_LOGDEBUG,
                         "accepted() handler returned false");

            /* closed() will not be called if accepted() fails. */

            Py_DECREF(y);
            y = NULL;
            ret = -1;
        }

        Py_DECREF(z);
    }

    /* The original user data is still retained by the listener. */

    sock->user_ = y;
    return ret;
}

static void
connected_(struct mod_handle* sock, const char* name)
{
    struct import_* import = mod_getsession()->user_;
    assert(import);
    assert(sock->user_);

    if (import->connected_) {

        PyObject* x = sock->user_;
        PyObject* y = PyObject_CallFunction(import->connected_, "Os", x,
                                            name);

        if (y) {
            Py_DECREF(y);
        } else
            printerr_();
    }

    /* closed() will always be called, even if connected() fails. */
}

static void
data_(const struct mod_handle* sock, const void* buf, size_t len)
{
    struct import_* import = mod_getsession()->user_;
    assert(import);
    assert(sock->user_);

    if (import->data_) {

        PyObject* x = sock->user_;
        PyObject* y = PyBuffer_FromMemory((void*)buf, (int)len);
        PyObject* z = PyObject_CallFunction(import->data_, "OO", x, y);

        if (z) {
            Py_DECREF(z);
        } else
            printerr_();

        Py_DECREF(y);
    }
}

static void
rdexpire_(const struct mod_handle* sock, unsigned* ms)
{
    struct import_* import = mod_getsession()->user_;
    assert(import);
    assert(sock->user_);

    if (import->rdexpire_) {

        PyObject* x = sock->user_;
        PyObject* y = PyInt_FromLong(*ms);
        PyObject* z = PyObject_CallFunction(import->rdexpire_, "OO", x, y);

        if (z) {
            if (PyInt_Check(z)) {
                mod_writelog(MOD_LOGDEBUG,
                             "handler returned new timeout value");
                *ms = PyInt_AsLong(z);
            }
            Py_DECREF(z);
        } else
            printerr_();

        Py_DECREF(y);
    }
}

static void
wrexpire_(const struct mod_handle* sock, unsigned* ms)
{
    struct import_* import = mod_getsession()->user_;
    assert(import);
    assert(sock->user_);

    if (import->wrexpire_) {

        PyObject* x = sock->user_;
        PyObject* y = PyInt_FromLong(*ms);
        PyObject* z = PyObject_CallFunction(import->wrexpire_, "OO", x, y);

        if (z) {
            if (PyInt_Check(z)) {
                mod_writelog(MOD_LOGDEBUG,
                             "handler returned new timeout value");
                *ms = PyInt_AsLong(z);
            }
            Py_DECREF(z);
        } else
            printerr_();

        Py_DECREF(y);
    }
}

static void
expire_(const struct mod_handle* timer, unsigned* ms)
{
    struct import_* import = mod_getsession()->user_;
    assert(import);
    assert(timer->user_);

    if (import->expire_) {

        PyObject* x, * y, * z;
        if (!(x = augpy_getblob((aug_object*)timer->user_))) {
            x = Py_None;
            Py_INCREF(x);
        }

        y = PyInt_FromLong(*ms);
        z = PyObject_CallFunction(import->expire_, "OO", x, y);
        Py_DECREF(y);
        Py_DECREF(x);

        if (z) {
            if (PyInt_Check(z)) {
                mod_writelog(MOD_LOGDEBUG,
                             "handler returned new timeout value");
                *ms = PyInt_AsLong(z);
            }
            Py_DECREF(z);
        } else
            printerr_();

        /* x will be Py_DECREF()-ed by destroy_() when *ms == 0. */
    }
}

static int
authcert_(const struct mod_handle* sock, const char* subject,
          const char* issuer)
{
    struct import_* import = mod_getsession()->user_;
    int ret = 0;
    assert(import);
    assert(sock->user_);

    if (import->authcert_) {

        PyObject* x = sock->user_;
        PyObject* y = PyObject_CallFunction(import->authcert_, "Oss", x,
                                            subject, issuer);
        if (y) {
            if (y == Py_False)
                ret = -1;
            Py_DECREF(y);
        } else {
            printerr_();
            ret = -1;
        }
    }

    return ret;
}

static const struct mod_module module_ = {
    stop_,
    start_,
    reconf_,
    event_,
    closed_,
    teardown_,
    accepted_,
    connected_,
    data_,
    rdexpire_,
    wrexpire_,
    expire_,
    authcert_
};

static const struct mod_module*
init_(const char* name)
{
    mod_writelog(MOD_LOGINFO, "initialising augpy module");

    if (initpy_() < 0)
        return NULL;

    return &module_;
}

static void
term_(void)
{
    mod_writelog(MOD_LOGINFO, "terminating augpy module");
    termpy_();
}

MOD_ENTRYPOINTS(init_, term_)
