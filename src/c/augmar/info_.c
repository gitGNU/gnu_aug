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
#define AUGMAR_BUILD
#include "augmar/info_.h"
#include "augctx/defs.h"

AUG_RCSID("$Id$");

#include "augmar/format_.h"
#include "augctx/base.h"
#include "augctx/errinfo.h"

#include <assert.h>
#include <string.h> /* memcpy() */

AUG_EXTERNC aug_result
aug_setinfo_(aug_seq_t seq, const struct aug_info_* info)
{
    char* addr;

    assert(seq && info);

    if (aug_setregion_(seq, 0, AUG_LEADERSIZE) < 0
        || !(addr = (char*)aug_seqaddr_(seq)))
        return -1;

    memcpy(addr + AUG_MAGICOFF, AUG_MAGIC, sizeof(aug_magic_t));
    aug_encodeproto((aug_proto_t)info->proto_, addr + AUG_PROTOOFF);
    aug_encodefields((aug_fields_t)info->fields_, addr + AUG_FIELDSOFF);
    aug_encodehsize((aug_hsize_t)info->hsize_, addr + AUG_HSIZEOFF);
    aug_encodebsize((aug_bsize_t)info->bsize_, addr + AUG_BSIZEOFF);

    return 0;
}

AUG_EXTERNC aug_result
aug_info_(aug_seq_t seq, struct aug_info_* info)
{
    char* addr;
    assert(seq && info);

    if (aug_setregion_(seq, 0, AUG_LEADERSIZE) < 0
        || !(addr = (char*)aug_seqaddr_(seq)))
        return -1;

    if (0 != memcmp(addr + AUG_MAGICOFF, AUG_MAGIC, sizeof(aug_magic_t))) {
        aug_setctxerror(aug_tlx, __FILE__, __LINE__, "aug", AUG_EINVAL,
                        AUG_MSG("invalid mar header"));
        return -1;
    }

    info->proto_ = aug_decodeproto(addr + AUG_PROTOOFF);
    info->fields_ = aug_decodefields(addr + AUG_FIELDSOFF);
    info->hsize_ = aug_decodehsize(addr + AUG_HSIZEOFF);
    info->bsize_ = aug_decodebsize(addr + AUG_BSIZEOFF);

    return 0;
}
