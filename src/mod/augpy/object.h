
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
#ifndef AUGPY_OBJECT_H
#define AUGPY_OBJECT_H

#if defined(_WIN32)
# undef _DEBUG
#endif /* _WIN32 */

#include "augmod.h"
#include "augext/blob.h"

#include <Python.h>

AUG_INTERFACE(augpy_box);

struct augpy_boxvtbl {
    AUG_VTBL(augpy_box);
    PyObject* (*unbox_)(augpy_box*);
};

augpy_box*
augpy_createbox(PyObject* pyob);

aug_blob*
augpy_createblob(PyObject* pyob);

PyObject*
augpy_obtopy(aug_object* ob);

PyTypeObject*
augpy_createtype(void);

PyObject*
augpy_createhandle(PyTypeObject* type, mod_id id, PyObject* ob);

augpy_box*
augpy_boxhandle(PyTypeObject* type, mod_id id, PyObject* ob);

void
augpy_setid(PyObject* self, mod_id id);

mod_id
augpy_getid(PyObject* self);

void
augpy_setob(PyObject* self, PyObject* ob);

PyObject*
augpy_getob(PyObject* self);

int
augpy_handles(void);

#endif /* AUGPY_OBJECT_H */
