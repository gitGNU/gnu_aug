/* Copyright (c) 2004-2007, Mark Aylett <mark@emantic.co.uk>
   See the file COPYING for copying permission.
*/
#ifndef AUGPY_MODULE_H
#define AUGPY_MODULE_H

#if defined(_WIN32)
# undef _DEBUG
#endif /* _WIN32 */

#include <Python.h>

PyObject*
augpy_createmodule(PyTypeObject* type);

#endif /* AUGPY_MODULE_H */